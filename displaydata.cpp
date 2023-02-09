#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cmBRadNum->addItem("Radar " + QString::number(i));

    ui->cBChsDist->setCurrentIndex(5); // set 250m

    ui->wDraw->colors = &this->colors;
    colors = std::vector<QColor>(ui->gridTypes->rowCount(), Qt::gray);
    if(colors.size() >= 8){
        colors[0] = Qt::red;
        colors[1] = Qt::yellow;
        colors[2] = Qt::blue;
        colors[3] = Qt::green;
        colors[4] = Qt::darkGray;
        colors[5] = Qt::darkGreen;
        colors[6] = QColor(255, 140, 0);
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
    // --- click ---
    on_cBInfo_clicked(true);
    ui->cBInfo->setChecked(true);

    // ---
    ui->wDraw->configInfo = &configRadar;

    // --- Config ---
    dConfig = new DialogConfig(this);
    dConfig->configRadar = &configRadar;
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(const CanLine &canLine){
    if(canLine.messId[0] == '6' && canLine.messId[2] == '0'){
        clustersAll.clear();
        numExpectNear = Converter::getDecData(canLine.messData, 0, 8);
        numExpectFar = Converter::getDecData(canLine.messData, 8, 8);
        numExpectSumm = numExpectNear + numExpectFar;
        measCount = Converter::getDecData(canLine.messData, 16, 16);
        //std::cout << GET_CUR_TIME_MILLI << " | " << (int)numExpectSumm << std::endl;
    }
    if(canLine.messId[0] == '7' && canLine.messId[2] == '1'){
        ClusterInfo cluster;

        // ID
        cluster.id = Converter::getDecData(canLine.messData, 0, 8);

        // RCS
        cluster.RCS = Converter::getDecData(canLine.messData, 56, 8);
        cluster.RCS *= resRCS;
        cluster.RCS += offsetRCS;

        // VRelLong
        cluster.vRelLong = Converter::getDecData(canLine.messData, 32, 10);
        cluster.vRelLong *= resVRelLong;
        cluster.vRelLong += offsetVRelLong;

        // VRelLat
        cluster.vRelLat = Converter::getDecData(canLine.messData, 42, 9);
        cluster.vRelLat *= resVRelLat;
        cluster.vRelLat += offsetVRelLat;

        // DistLong
        cluster.distLong = Converter::getDecData(canLine.messData, 8, 13);
        cluster.distLong *= resDistLong;
        cluster.distLong += offsetDistLong;

        // DistLat
        cluster.distLat = Converter::getDecData(canLine.messData, 22, 10);
        cluster.distLat *= resDistLat;
        cluster.distLat += offsetDistLat;
        cluster.distLat = -cluster.distLat;// NOTE: left/right?

        // Type
        uint8_t numType = Converter::getDecData(canLine.messData, 53, 3);
        cluster.type = static_cast<ClusterDynProp>(numType);

        // Azimuth
        cluster.clacAzimuth();

        // ---
        clustersAll.push_back(cluster);
    }
    if(canLine.messId[0] == '7' && canLine.messId[2] == '2'){
        uint8_t idCl = Converter::getDecData(canLine.messData, 0, 8);
        uint8_t pDh0Cl = Converter::getDecData(canLine.messData, 29, 3);
        if(clustersAll.size() > idCl){
            switch (pDh0Cl) {
            case 0:
                clustersAll[idCl].Pdh0 = 0.0f;
                break;
            case 1:
                clustersAll[idCl].Pdh0 = 24.99f;
                break;
            case 2:
                clustersAll[idCl].Pdh0 = 49.99f;
                break;
            case 3:
                clustersAll[idCl].Pdh0 = 74.99f;
                break;
            case 4:
                clustersAll[idCl].Pdh0 = 89.99f;
                break;
            case 5:
                clustersAll[idCl].Pdh0 = 98.99f;
                break;
            case 6:
                clustersAll[idCl].Pdh0 = 99.89f;
                break;
            case 7:
                clustersAll[idCl].Pdh0 = 100.0f;
                break;
            default:
                break;
            }
        }
    }
    if(canLine.messId[0] == '2' && canLine.messId[2] == '1'){
        // --- can num ---
        configRadar.canNum = std::atoi(&canLine.canNum.toStdString().back());

        // --- write ---
        configRadar.writeStatus = Converter::getDecData(canLine.messData, 0, 1);

        // --- read ---
        configRadar.readStatus = Converter::getDecData(canLine.messData, 1, 1);

        // --- dist ---
        configRadar.setFarZone(Converter::getDecData(canLine.messData, 8, 10));
        configRadar.setFarZone(configRadar.getFarZone() * resMaxDist);

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
    if(canLine.messId[0] == '3' && canLine.messId[2] == '0'){
        float motionSpeed = Converter::getDecData(canLine.messData, 3, 13);
        motionSpeed *= 0.02f;
        std::cout << "Motion speed| " << Converter::floatCutOff(motionSpeed, 1).toStdString() << std::endl;
    }

    // --- frame got ---
    if((int)clustersAll.size() == numExpectSumm){
        calcSpeed();
        showSpeedUI();

        // NOTE: Send frame to visual
        applyFilters();        
        updateShowFlags();

        ui->wDraw->clusters = clustersFiltered;
        ui->wDraw->measCount = measCount;
        ui->wDraw->numClNear = numExpectNear;
        ui->wDraw->numClFar = numExpectFar;
        ui->wDraw->numClSumm = numExpectSumm;
        ui->wDraw->update();
        // ---
        clustersAll.clear();
    }
}

void DisplayData::applyFilters(){
    clustersFiltered.clear();
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
    /*std::cout << "Win:" << windowTitle().toStdString() << " | Before: "
     << clustersAll.size() << " | After: " << clustersFiltered.size() << std::endl;*/
}

void DisplayData::updateShowFlags(){
    int sz = ui->gridProps->rowCount();
    ui->wDraw->props.resize(sz);
    for (int i = 0; i < sz; i++) {
        QRadioButton* rb = static_cast<QRadioButton*>(ui->gridProps->itemAtPosition(i, 0)->widget());
        ui->wDraw->props[i] = rb->isChecked();
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
            if (m_dataInfo[a].type == ClusterDynProp::oncoming){
                val = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[val]++;
            }
        }
    }
    if(statusSpeed == StatusSpeed::slowSpeed){
        for (uint8_t a = 1; a < 0xFF; a++){
            if (m_dataInfo[a].type == ClusterDynProp::oncoming ||
                    m_dataInfo[a].type == ClusterDynProp::stationary ||
                    m_dataInfo[a].type == ClusterDynProp::moving){
                val = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[val]++;
            }
        }
    }
    if(statusSpeed == StatusSpeed::backward){
        for (uint8_t a = 1; a < 0xFF; a++){
            if (m_dataInfo[a].type == ClusterDynProp::moving){
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

    if (valMax > 8 && numExpectSumm > 8){
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

void DisplayData::on_cBChsDist_currentTextChanged(const QString &data){
    ui->wDraw->aspect = 100.0f / data.toFloat();
    ui->wDraw->resizeAspect();
}

void DisplayData::on_cmBRadNum_currentIndexChanged(int index){
    configRadar.index = index;
    clustersFiltered.clear();
}

void DisplayData::on_cBInfo_clicked(bool checked){
    ui->wDraw->isShowInfo = checked;
}

void DisplayData::on_pBConfigRadar_clicked(){
    dConfig->setWindowTitle("Radar " + QString::number(configRadar.index));
    dConfig->show();
}
