#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBRadNum->addItem("Radar " + QString::number(i));

    ui->wDraw->colors = &this->colors;
    colors = std::vector<QColor>(ui->gridTypes->rowCount(), Qt::gray);
    if(colors.size() >= 8){
        colors[0] = Qt::red;
        colors[1] = Qt::yellow;
        colors[2] = Qt::blue;
        colors[3] = QColor(70, 220, 70); // green
        colors[4] = Qt::darkGray;
        colors[5] = Qt::darkGreen;
        colors[6] = QColor(255, 140, 0); // orange
        colors[7] = Qt::black;
    }

    // set colors
    int sz = 15;
    QPixmap px(sz, sz);
    for (int i = 0; i < ui->gridTypes->rowCount(); i++) {
        px.fill(colors[i]);
        QToolButton* tButton = static_cast<QToolButton*>(ui->gridTypes->itemAtPosition(i, 0)->widget());
        tButton->setIcon(px);
    }
    // ---
    ui->wDraw->configInfo = &configRadar;

    // --- config ---
    dConfig = new DialogConfig(this);
    dConfig->configRadar = &configRadar;

    // --- connections ---
    connect(ui->pBConfigRadar, SIGNAL(clicked()), this, SLOT(configRadarCall()));
    connect(ui->cBInfo, SIGNAL(clicked(bool)), this, SLOT(info(bool)));
    connect(ui->cBRadNum, SIGNAL(currentIndexChanged(int)), this, SLOT(radNum(int)));
    connect(ui->cBChsDist, SIGNAL(currentTextChanged(QString)), this, SLOT(chooseDist(QString)));

    // --- click ---
    ui->cBInfo->click();
    ui->cBChsDist->setCurrentIndex(5); // set 250m
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(const CanLine &canLine){
    // --- ------ CLUSTERS --- --- ---
    if(canLine.messId[0] == '6' && canLine.messId[2] == '0'){ // CLUST LIST
        calcSpeed();
        showSpeedUI();

        // NOTE: Send cluster frame to visual
        updateShowFlags();
        applyFilters();

        ui->wDraw->clusters = clustersFiltered;
        ui->wDraw->clustList = clustList;
        ui->wDraw->update();
        // ---
        clustersAll.clear();
        // ---
        clustList.numExpectNear = Converter::getDecData(canLine.messData, 0, 8);
        clustList.numExpectFar = Converter::getDecData(canLine.messData, 8, 8);
        clustList.numExpectSumm = clustList.numExpectNear + clustList.numExpectFar;
        clustList.measCount = Converter::getDecData(canLine.messData, 16, 16);
        clustList.interfVers = Converter::getDecData(canLine.messData, 32, 4);
    }
    if(canLine.messId[0] == '7' && canLine.messId[2] == '0'){ // SOFT VERSION
        versID->major = Converter::getDecData(canLine.messData, 0, 8);
        versID->minor = Converter::getDecData(canLine.messData, 8, 8);
        versID->patch = Converter::getDecData(canLine.messData, 16, 8);
        versID->extended = Converter::getDecData(canLine.messData, 30, 1);
        versID->country = Converter::getDecData(canLine.messData, 31, 1);
    }
    if(canLine.messId[0] == '7' && canLine.messId[2] == '1'){ // GENERAL INFO
        ClusterInfo cluster;
        // ID
        cluster.id = Converter::getDecData(canLine.messData, 0, 8);
        // RCS
        cluster.RCS = Converter::getDecData(canLine.messData, 56, 8);
        cluster.RCS *= resClustRCS;
        cluster.RCS += offsetClustRCS;
        // VRelLong
        cluster.vRelLong = Converter::getDecData(canLine.messData, 32, 10);
        cluster.vRelLong *= resClustVRelLong;
        cluster.vRelLong += offsetClustVRelLong;
        // VRelLat
        cluster.vRelLat = Converter::getDecData(canLine.messData, 42, 9);
        cluster.vRelLat *= resClustVRelLat;
        cluster.vRelLat += offsetClustVRelLat;
        // DistLong
        cluster.distLong = Converter::getDecData(canLine.messData, 8, 13);
        cluster.distLong *= resClustDistLong;
        cluster.distLong += offsetClustDistLong;
        // DistLat
        cluster.distLat = Converter::getDecData(canLine.messData, 22, 10);
        cluster.distLat *= resClustDistLat;
        cluster.distLat += offsetClustDistLat;
        cluster.distLat = -cluster.distLat;// NOTE: clusters left/right?
        // Type
        uint8_t numType = Converter::getDecData(canLine.messData, 53, 3);
        cluster.type = static_cast<DynProp>(numType);
        // Azimuth
        cluster.clacAzimuth();
        // ---
        clustersAll.push_back(cluster);
    }
    if(canLine.messId[0] == '7' && canLine.messId[2] == '2'){ // QUALITY
        uint8_t idCl = Converter::getDecData(canLine.messData, 0, 8);
        for(auto &cl : clustersAll)
            if(idCl == cl.id){
                uint8_t pDh0Cl = Converter::getDecData(canLine.messData, 29, 3);
                switch (pDh0Cl) {
                case 0:
                    cl.Pdh0 = 0.0f;
                    break;
                case 1:
                    cl.Pdh0 = 24.99f;
                    break;
                case 2:
                    cl.Pdh0 = 49.99f;
                    break;
                case 3:
                    cl.Pdh0 = 74.99f;
                    break;
                case 4:
                    cl.Pdh0 = 89.99f;
                    break;
                case 5:
                    cl.Pdh0 = 98.99f;
                    break;
                case 6:
                    cl.Pdh0 = 99.89f;
                    break;
                case 7:
                    cl.Pdh0 = 100.0f;
                    break;
                default:
                    break;
                }
            }
    }

    // --- RADAR STATE & FILTERS---
    if(canLine.messId[0] == '2' && canLine.messId[2] == '1'){ // RADAR STATE
        // --- can num ---
        configRadar.canNum = std::atoi(&canLine.canNum.toStdString().back());

        // --- write ---
        configRadar.writeStatus = Converter::getDecData(canLine.messData, 0, 1);

        // --- read ---
        configRadar.readStatus = Converter::getDecData(canLine.messData, 1, 1);

        // --- dist ---
        configRadar.setFarZone(Converter::getDecData(canLine.messData, 8, 10));
        configRadar.setFarZone(configRadar.getFarZone() * resConfRadMaxDist);

        // --- pers error ---
        configRadar.persistErr = Converter::getDecData(canLine.messData, 18, 1);

        // --- interfer ---
        configRadar.interference = Converter::getDecData(canLine.messData, 19, 1);

        // --- temper ---
        configRadar.temperatErr = Converter::getDecData(canLine.messData, 20, 1);

        // --- tempor ---
        configRadar.temporarErr = Converter::getDecData(canLine.messData, 21, 1);

        // --- volt ---
        configRadar.voltErr = Converter::getDecData(canLine.messData, 22, 1);

        // --- sort ---
        configRadar.sortInd = Converter::getDecData(canLine.messData, 33, 3);

        // --- power ---
        configRadar.power = Converter::getDecData(canLine.messData, 30, 3);

        // --- relay ---
        configRadar.relay = Converter::getDecData(canLine.messData, 46, 1);

        // --- output type ---
        configRadar.outputType = Converter::getDecData(canLine.messData, 44, 2);

        // --- send qual ---
        configRadar.sendQual = Converter::getDecData(canLine.messData, 43, 1);

        // --- send ext ---
        configRadar.sendExt = Converter::getDecData(canLine.messData, 42, 1);

        // --- motion ---
        configRadar.motionRxState = Converter::getDecData(canLine.messData, 40, 2);

        // --- threshold ---
        configRadar.thrRcs = Converter::getDecData(canLine.messData, 59, 3);

        // --- --- ---
        dConfig->updateUI();
    }
    if(canLine.messId[0] == '2' && canLine.messId[2] == '3'){ // Filters
        dConfig->is203Got = true;
        dConfig->fltClust = Converter::getDecData(canLine.messData, 0, 5);
        dConfig->fltObj = Converter::getDecData(canLine.messData, 8, 5);
        dConfig->updateUI();
    }

    // --- ------ OBJECTS --- --- ---
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'a' || canLine.messId[2] == 'A')){ // OBJS LIST
        // NOTE: Send object frame to visual
        updateShowFlags();
        applyFilters();

        ui->wDraw->objects = objectsFiltered;
        ui->wDraw->objList = objList;
        ui->wDraw->update();
        // ---
        objectsAll.clear();
        // ---
        objList.numExpect = Converter::getDecData(canLine.messData, 0, 8);
        objList.measCount = Converter::getDecData(canLine.messData, 8, 16);
        objList.interfVers = Converter::getDecData(canLine.messData, 24, 4);
    }
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'b' || canLine.messId[2] == 'B')){ // GENERAL INFO
        ObjectInfo object;
        // ID
        object.id = Converter::getDecData(canLine.messData, 0, 8);
        // RCS
        object.RCS = Converter::getDecData(canLine.messData, 56, 8);
        object.RCS *= resObjRCS;
        object.RCS += offsetObjRCS;
        // VRelLong
        object.vRelLong = Converter::getDecData(canLine.messData, 32, 10);
        object.vRelLong *= resObjVRelLong;
        object.vRelLong += offsetObjVRelLong;
        // VRelLat
        object.vRelLat = Converter::getDecData(canLine.messData, 42, 9);
        object.vRelLat *= resObjVRelLat;
        object.vRelLat += offsetObjVRelLat;
        // DistLong
        object.distLong = Converter::getDecData(canLine.messData, 8, 13);
        object.distLong *= resObjDistLong;
        object.distLong += offsetObjDistLong;
        // DistLat
        object.distLat = Converter::getDecData(canLine.messData, 21, 11);
        object.distLat *= resObjDistLat;
        object.distLat += offsetObjDistLat;
        object.distLat = -object.distLat; // NOTE: objects left/right?
        // Type
        uint8_t numType = Converter::getDecData(canLine.messData, 53, 3);
        object.type = static_cast<DynProp>(numType);
        // Azimuth
        object.clacAzimuth();
        // ---
        objectsAll.push_back(object);
    }
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'c' || canLine.messId[2] == 'C')){ // QUALITY
        uint8_t idObj = Converter::getDecData(canLine.messData, 0, 8);
        uint8_t pDh0Obj = Converter::getDecData(canLine.messData, 48, 3);
        if(objectsAll.size() > idObj){
            switch (pDh0Obj) {
            case 0:
                objectsAll[idObj].Pdh0 = 0.0f;
                break;
            case 1:
                objectsAll[idObj].Pdh0 = 24.99f;
                break;
            case 2:
                objectsAll[idObj].Pdh0 = 49.99f;
                break;
            case 3:
                objectsAll[idObj].Pdh0 = 74.99f;
                break;
            case 4:
                objectsAll[idObj].Pdh0 = 89.99f;
                break;
            case 5:
                objectsAll[idObj].Pdh0 = 98.99f;
                break;
            case 6:
                objectsAll[idObj].Pdh0 = 99.89f;
                break;
            case 7:
                objectsAll[idObj].Pdh0 = 100.0f;
                break;
            default:
                break;
            }
        }
    }
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'd' || canLine.messId[2] == 'D')){ // EXTENDED
        uint8_t idObj = Converter::getDecData(canLine.messData, 0, 8);
        for(auto& obj : objectsAll)
            if(idObj == obj.id){
                // Class
                uint8_t clss = Converter::getDecData(canLine.messData, 29, 3);
                obj.objClass = static_cast<ObjectClass>(clss);
                // Angle
                obj.angle = Converter::getDecData(canLine.messData, 32, 10);
                obj.angle *= resObjAngle;
                obj.angle += offsetObjAngle;
                // Length
                obj.length = Converter::getDecData(canLine.messData, 48, 8);
                obj.length *= resObjLenght;
                obj.length += offsetObjLenght;
                // Width
                obj.width = Converter::getDecData(canLine.messData, 56, 8);
                obj.width *= resObjWidth;
                obj.width += offsetObjWidth;
            }
    }
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'e' || canLine.messId[2] == 'E')){ // COLLISION

    }
}

void DisplayData::applyFilters(){
    clustersFiltered.clear();
    objectsFiltered.clear();
    // ---
    std::vector<bool> types{ui->cBClMov->isChecked(), ui->cBClStat->isChecked(),
                ui->cBClOnCom->isChecked(), ui->cBClStatCond->isChecked(),
                ui->cBClUnkn->isChecked(), ui->cBClCrossStat->isChecked(),
                ui->cBClCrossMov->isChecked(), ui->cBClStop->isChecked()};
    // ---
    int minRcs = ui->sBRcsMin->value();
    int maxRcs = ui->sBRcsMax->value();
    int minDLong = ui->sBDistLongMin->value();
    int maxDLong = ui->sBDistLongMax->value();
    int minDLat = ui->sBDistLatMin->value();
    int maxDLat = ui->sBDistLatMax->value();
    int minVLong = ui->sBVelLongMin->value();
    int maxVLong = ui->sBVelLongMax->value();
    int minVLat = ui->sBVelLatMin->value();
    int maxVLat = ui->sBVelLatMax->value();
    int minPdh0 = ui->sBPdh0Min->value();
    int maxPdh0 = ui->sBPdh0Max->value();
    int minAzmth = ui->sBAzMin->value();
    int maxAzmth = ui->sBAzMax->value();

    for(const auto &cl : clustersAll){
        if((cl.RCS >= minRcs && cl.RCS <= maxRcs) && (cl.distLong >= minDLong && cl.distLong <= maxDLong)
                && (cl.distLat >= minDLat && cl.distLat <= maxDLat) &&(cl.vRelLong >= minVLong && cl.vRelLong <= maxVLong)
                && (cl.vRelLat >= minVLat && cl.vRelLat <= maxVLat) && (cl.azimuth >= minAzmth && cl.azimuth <= maxAzmth)
                && (cl.Pdh0 >= minPdh0 && cl.Pdh0 <= maxPdh0) && (types[static_cast<uint8_t>(cl.type)]))
            clustersFiltered.push_back(cl);
    }
    for(const auto &obj : objectsAll){
        if((obj.RCS >= minRcs && obj.RCS <= maxRcs) && (obj.distLong >= minDLong && obj.distLong <= maxDLong)
                && (obj.distLat >= minDLat && obj.distLat <= maxDLat) &&(obj.vRelLong >= minVLong && obj.vRelLong <= maxVLong)
                && (obj.vRelLat >= minVLat && obj.vRelLat <= maxVLat) && (obj.azimuth >= minAzmth && obj.azimuth <= maxAzmth)
                && (obj.Pdh0 >= minPdh0 && obj.Pdh0 <= maxPdh0) && (types[static_cast<uint8_t>(obj.type)]))
            objectsFiltered.push_back(obj);
    }
}

void DisplayData::updateShowFlags(){
    int sz = ui->gridProps->rowCount();
    ui->wDraw->properties.resize(sz);
    for (int i = 0; i < sz; i++) {
        QRadioButton* rb = static_cast<QRadioButton*>(ui->gridProps->itemAtPosition(i, 0)->widget());
        ui->wDraw->properties[i] = rb->isChecked();
    }
}

int DisplayData::calcSpeed(){
    uint8_t posMax = 127;
    uint8_t valMax = 0;
    uint8_t val = 0;
    float koeff = 60.0f;
    float averSpeedRes = 0.0f;
    float qualitySpeed = 60.0f;
    uint8_t* pHistoArray = histoVLong.data();
    uint8_t pointMax = 0;
    uint8_t point1LMax = 0;
    uint8_t point2LMax = 0;
    uint8_t point1RMax = 0;
    uint8_t point2RMax = 0;

    histoVLong.fill(0x00);

    for (size_t i = 0; i < clustersAll.size(); i++)
        m_dataInfo[i] = clustersAll[i];

    if(statusSpeed == StatusSpeed::forward){
        for (uint8_t a = 1; a < 0xFF; a++){
            if (m_dataInfo[a].type == DynProp::oncoming){
                val = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[val]++;
            }
        }
    }
    if(statusSpeed == StatusSpeed::slowSpeed){
        for (uint8_t a = 1; a < 0xFF; a++){
            if (m_dataInfo[a].type == DynProp::oncoming ||
                    m_dataInfo[a].type == DynProp::stationary ||
                    m_dataInfo[a].type == DynProp::moving){
                val = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[val]++;
            }
        }
    }
    if(statusSpeed == StatusSpeed::backward){
        for (uint8_t a = 1; a < 0xFF; a++){
            if (m_dataInfo[a].type == DynProp::moving){
                val = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[val]++;
            }
        }
    }

    for (uint8_t i = 4; i < 0xFF - 4; i++){
        if (pHistoArray[i] > 0){
            if (valMax < pHistoArray[i]){
                valMax = pHistoArray[i];
                posMax = i;
            }
        }
    }

    pointMax = pHistoArray[posMax];
    point1LMax = pHistoArray[posMax-1];
    point2LMax = pHistoArray[posMax-2];
    point1RMax = pHistoArray[posMax+1];
    point2RMax = pHistoArray[posMax+2];

    valMax = point1LMax + pointMax + point1RMax;

    if (point1LMax + point1RMax)
        koeff = static_cast<float>(pointMax) / static_cast<float>(point1LMax + point1RMax);

    if (valMax > 8 && clustList.numExpectSumm > 8){
        float averSpeed4 = 0.0f;
        float valMax4 = 0.0f;

        float averSpeed = (point1LMax * (posMax - 1) + pointMax * (posMax) + point1RMax * (posMax + 1))
                / static_cast<float>(point1LMax+pointMax+point1RMax);

        if (point1LMax > point1RMax){
            valMax4 = point2LMax + point1LMax + pointMax + point1RMax;
            averSpeed4 = (point2LMax * (posMax - 2) + point1LMax * (posMax - 1) + pointMax * (posMax) + point1RMax * (posMax + 1))
                    / static_cast<float>(valMax4);
        }
        else{
            valMax4 = point1LMax + pointMax + point1RMax + point2RMax;

            averSpeed4 = (point1LMax * (posMax - 1) + pointMax * (posMax) + point1RMax * (posMax + 1) + point2RMax * (posMax + 2))
                    / static_cast<float>(valMax4);
        }

        if (koeff > 3) averSpeedRes = averSpeed;
        else averSpeedRes = averSpeed4;

        if (fabs(averSpeedKalman - 127) > 0.01f){
            averSpeedKalman = (averSpeedRes * koeffKalmanSpeed + averSpeedKalman * (1-koeffKalmanSpeed));
        }
        else{
            averSpeedKalman = averSpeedRes;
        }

        if (posMax >= zonaAnalyseQuality && posMax < DATA_SIZE - zonaAnalyseQuality){
            float var3 = 0.0f;
            float summ3 = 0.0f;
            pHistoArray[posMax - 1] += 2;
            pHistoArray[posMax + 1] += 2;

            for (int16_t i = -zonaAnalyseQuality; i <= zonaAnalyseQuality; ++i){
                var3 += pHistoArray[posMax + i] * i*i;
                summ3 += pHistoArray[posMax + i];
            }
            var3 /= summ3;
            qualitySpeed = (5 * var3) / valMax;
        }
        else{
            qualitySpeed = 60.0f;
        }

        if (averQuality < 59)
            averQuality = (1 - koeffKalmanQuality)*averQuality + koeffKalmanQuality*(qualitySpeed);
        else
            averQuality = qualitySpeed;

        speedVehicle = (averSpeedKalman - 127.f) * 0.25f;
        speedQuality = averQuality;
        // std::cout << static_cast<int32_t>(valMax) << " : " << static_cast<int32_t>(posMax)
    }
    else{
        speedQuality = 60.0f;
        speedVehicle = 0;
        return -1;
    }
    return 0;
}

void DisplayData::showSpeedUI(){
    ui->lSpeed_M_KM->setText("Speed: " + Converter::floatCutOff(speedVehicle, 1) + "m/s (" +
                             Converter::floatCutOff(speedVehicle * 3.6f, 1) + "km/h)");
}

void DisplayData::chooseDist(const QString &data){
    ui->wDraw->aspect = 100.0f / data.toFloat();
    ui->wDraw->resizeAspect();
}

void DisplayData::radNum(int index){
    configRadar.id = index;
    clustersFiltered.clear();
}

void DisplayData::info(bool checked){
    ui->wDraw->isShowInfo = checked;
}

void DisplayData::configRadarCall(){
    dConfig->setWindowTitle("Radar " + QString::number(configRadar.id));
    dConfig->show();
}
