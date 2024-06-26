#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    const int w = 400; // 400...2000
    vDraw = new VisImage(this);
    vDraw->setFixedSize(w, w / aspect);
    ui->sAVisual->setWidget(vDraw);
    ui->sAVisual->verticalScrollBar()->setValue(vDraw->height());

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBRadNum->addItem("Radar " + QString::number(i));

    // --- colors properties ---
    vDraw->colorsDynProp = std::vector<QColor>(ui->gridTypes->rowCount(), Qt::gray);
    if(vDraw->colorsDynProp.size() >= 8){
        vDraw->colorsDynProp[0] = Qt::red;
        vDraw->colorsDynProp[1] = Qt::yellow;
        vDraw->colorsDynProp[2] = Qt::blue;
        vDraw->colorsDynProp[3] = QColor(70, 220, 70); // green
        vDraw->colorsDynProp[4] = Qt::darkGray;
        vDraw->colorsDynProp[5] = Qt::darkGreen;
        vDraw->colorsDynProp[6] = QColor(255, 140, 0); // orange
        vDraw->colorsDynProp[7] = Qt::black;
    }

    // set colors
    int sz = 50;
    QPixmap px(sz, sz);
    for (int i = 0; i < ui->gridTypes->rowCount(); i++) {
        px.fill(vDraw->colorsDynProp[i]);
        QToolButton* tButton = static_cast<QToolButton*>(ui->gridTypes->itemAtPosition(i, 0)->widget());
        tButton->setIcon(px);
    }

    // --- config ---
    dConfig = new DialogConfig(this);

    // --- bindings ---
    uint8_t transp = 140;
    vDraw->colorsWarnLevel[0] = QColor(160, 255, 160, transp); // no warn
    vDraw->colorsWarnLevel[1] = QColor(255, 140, 140, transp); // warn
    vDraw->colorsWarnLevel[2] = QColor(128, 128, 128, transp); // unused
    vDraw->colorsWarnLevel[3] = QColor(255, 150, 50, transp); // left
    dConfig->colorsWarnLevel = &vDraw->colorsWarnLevel;
    vDraw->regions = &dConfig->regions;

    // --- connections ---
    connect(ui->pBConfigRadar, SIGNAL(clicked()), this, SLOT(slotConfigRadarCall()));
    connect(ui->pBPathReset, SIGNAL(clicked()), this, SLOT(slotPathReset()));
    connect(ui->cBInfo, SIGNAL(clicked(bool)), this, SLOT(slotInfo(bool)));
    connect(ui->cBRadNum, SIGNAL(currentIndexChanged(int)), this, SLOT(slotRadNum(int)));
    connect(this, SIGNAL(signRadarWarningsUI()), this, SLOT(slotUpdateWarningsUI()));
    connect(this, SIGNAL(signUpdRegionList()), dConfig, SLOT(updRegListUI()));
    // --- visual ---
    connect(ui->hSZoom, SIGNAL(valueChanged(int)), this, SLOT(slotZoomChanged(int)));
    connect(ui->sAVisual->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotSAVertChanged(int)));
    connect(ui->sAVisual->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotSAHorizChanged(int)));    

    // --- post events ---
    ui->cBInfo->click();

#ifdef __WIN32
    QFont font = ui->lSpeed_M_KM->font();
    font.setPointSize(10);
    ui->lSpeed_M_KM->setFont(font);
    ui->lPath_m->setFont(font);
#endif
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(const CanLine &canLine){
    // --- ------ CLUSTERS --- --- ---
    if(canLine.messId[0] == '6' && canLine.messId[2] == '0'){ // CLUST LIST
        calcSpeed();
        calcPath();

        if(speedVehicle >= HIGH_SPEED) statusSpeed = StatusSpeed::backward;
        if(speedVehicle < HIGH_SPEED && speedVehicle > LOW_SPEED) statusSpeed = StatusSpeed::slowSpeed;
        if(speedVehicle <= LOW_SPEED) statusSpeed = StatusSpeed::forward;
        showSpeedPathUI();

        // NOTE: Send cluster frame to visual
        updateShowFlags();
        applyFilters();

        vDraw->clusters = clustersFiltered;
        vDraw->clustList = clustList;
        vDraw->update();
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
        if(isMirrored)
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
                    cl.Pdh0 = 25.0f;
                    break;
                case 2:
                    cl.Pdh0 = 50.0f;
                    break;
                case 3:
                    cl.Pdh0 = 75.0f;
                    break;
                case 4:
                    cl.Pdh0 = 90.0f;
                    break;
                case 5:
                    cl.Pdh0 = 99.0f;
                    break;
                case 6:
                    cl.Pdh0 = 99.9f;
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
        dConfig->configRadar.canNum = std::atoi(&canLine.canNum.toStdString().back());

        // --- write ---
        dConfig->configRadar.writeStatus = Converter::getDecData(canLine.messData, 0, 1);

        // --- read ---
        dConfig->configRadar.readStatus = Converter::getDecData(canLine.messData, 1, 1);

        // --- dist ---
        dConfig->configRadar.setFarZone(Converter::getDecData(canLine.messData, 8, 10));
        dConfig->configRadar.setFarZone(dConfig->configRadar.getFarZone() * resConfRadMaxDist);

        // --- pers error ---
        dConfig->configRadar.persistErr = Converter::getDecData(canLine.messData, 18, 1);

        // --- interfer ---
        dConfig->configRadar.interference = Converter::getDecData(canLine.messData, 19, 1);

        // --- temper ---
        dConfig->configRadar.temperatErr = Converter::getDecData(canLine.messData, 20, 1);

        // --- tempor ---
        dConfig->configRadar.temporarErr = Converter::getDecData(canLine.messData, 21, 1);

        // --- volt ---
        dConfig->configRadar.voltErr = Converter::getDecData(canLine.messData, 22, 1);

        // --- sort ---
        dConfig->configRadar.sortInd = Converter::getDecData(canLine.messData, 33, 3);

        // --- power ---
        dConfig->configRadar.power = Converter::getDecData(canLine.messData, 30, 3);

        // --- relay ---
        dConfig->configRadar.relay = Converter::getDecData(canLine.messData, 46, 1);

        // --- output type ---
        dConfig->configRadar.outputType = Converter::getDecData(canLine.messData, 44, 2);

        // --- send qual ---
        dConfig->configRadar.sendQual = Converter::getDecData(canLine.messData, 43, 1);

        // --- send ext ---
        dConfig->configRadar.sendExt = Converter::getDecData(canLine.messData, 42, 1);

        // --- motion ---
        dConfig->configRadar.motionRxState = Converter::getDecData(canLine.messData, 40, 2);

        // --- threshold ---
        dConfig->configRadar.thrRcs = Converter::getDecData(canLine.messData, 59, 3);

        // --- --- ---
        vDraw->configRadar = dConfig->configRadar;
        dConfig->updateConfigUI();
        emit signRadarWarningsUI();
    }
    if(canLine.messId[0] == '2' && canLine.messId[2] == '3'){ // FILTERS
        dConfig->is203Got = true;
        dConfig->fltClust = Converter::getDecData(canLine.messData, 0, 5);
        dConfig->fltObj = Converter::getDecData(canLine.messData, 8, 5);
        dConfig->updateConfigUI();
    }

    // --- ------ OBJECTS --- --- ---
    if(canLine.messId[0] == '6' && (canLine.messId[2] == 'a' || canLine.messId[2] == 'A')){ // OBJS LIST
        // --- time for clearing ---
        uint64_t diffCollState = GET_CUR_TIME_MILLI - dConfig->prev408GotMs;
        if(diffCollState > dConfig->period408GotMs){
            if(dConfig->is408Got){
                dConfig->is408Got = false;
            }
            else{
                dConfig->regions.clear();
                emit signUpdRegionList();
            }
            dConfig->prev408GotMs = GET_CUR_TIME_MILLI;
        }

        // NOTE: Send object frame to visual
        updateShowFlags();
        applyFilters();

        vDraw->objects = objectsFiltered;
        vDraw->objList = objList;
        vDraw->update();
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
        if(isMirrored)
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
        uint8_t idObj = Converter::getDecData(canLine.messData, 0, 8);
        for(auto& obj : objectsAll)
            if(idObj == obj.id){
                uint8_t regBits = Converter::getDecData(canLine.messData, 8, 8);
                QString strBits = Converter::decToBin(QString::number(regBits), COLL_REG_NUM);
                for (int i = 0; i < strBits.length(); i++)
                    if(strBits[i] == QChar('1')) obj.collRegs[i] = true;
            }
    }

    // --- COLLISIONS ---
    if(canLine.messId[0] == '4' && canLine.messId[2] == '2'){ // REGION DATA
        // --- time for clearing ---
        uint64_t diffTimeReg = GET_CUR_TIME_MILLI - dConfig->prev402GotMs;
        if(diffTimeReg > dConfig->period402GotMs){
            dConfig->prev402GotMs = GET_CUR_TIME_MILLI;
            dConfig->regions.clear();
        }
        // ---
        CollRegion collRegion;
        // --- id ---
        collRegion.id = Converter::getDecData(canLine.messData, 0, 3);
        // --- warn level ---
        uint8_t warn = Converter::getDecData(canLine.messData, 3, 2);
        collRegion.warnLevel = static_cast<WarningLevel>(warn);
        // --- X1 ---
        collRegion.pt1X = Converter::getDecData(canLine.messData, 45, 11); // 21?
        collRegion.pt1X *= resCollPt1X;
        collRegion.pt1X += offsetCollPt1X;
        // --- Y1 ---
        collRegion.pt1Y = Converter::getDecData(canLine.messData, 8, 13);
        collRegion.pt1Y *= resCollPt1Y;
        collRegion.pt1Y += offsetCollPt1Y;
        // --- X2 ---
        collRegion.pt2X = Converter::getDecData(canLine.messData, 21, 11); // 45?
        collRegion.pt2X *= resCollPt2X;
        collRegion.pt2X += offsetCollPt2X;
        // --- Y2 ---
        collRegion.pt2Y = Converter::getDecData(canLine.messData, 32, 13);
        collRegion.pt2Y *= resCollPt2Y;
        collRegion.pt2Y += offsetCollPt2Y;
        // --- nof ---
        collRegion.nofObj = Converter::getDecData(canLine.messData, 56, 8);
        // ---
        dConfig->regions.push_back(collRegion);
        emit signUpdRegionList();
    }
    if(canLine.messId[0] == '4' && canLine.messId[2] == '8'){ // COLLISION STATE
        dConfig->is408Got = true;
        dConfig->collDetState.isActive = Converter::getDecData(canLine.messData, 6, 1);
        dConfig->collDetState.nofRegs = Converter::getDecData(canLine.messData, 0, 4);
        dConfig->collDetState.detectTimeSec = Converter::getDecData(canLine.messData, 8, 8);
        dConfig->collDetState.detectTimeSec *= resCollDetTime;
        dConfig->collDetState.measCount = Converter::getDecData(canLine.messData, 16, 16);
        dConfig->updateCollDetStateUI();
        // --- 0 regions left ---
        if(dConfig->collDetState.nofRegs == 0){
            dConfig->regions.clear();
            emit signUpdRegionList();
        }
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
    int flagSize = ui->gridProps->rowCount();
    vDraw->showProperties.resize(flagSize);
    for (int i = 0; i < flagSize; i++) {
        QRadioButton* rb = static_cast<QRadioButton*>(ui->gridProps->itemAtPosition(i, 0)->widget());
        vDraw->showProperties[i] = rb->isChecked();
    }
}

int DisplayData::calcSpeed(){
    uint8_t indMaxValInHist = 127;
    uint8_t valMaxInHist = 0;
    uint8_t valForHist = 0;

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

    if(statusSpeed == StatusSpeed::forward)
        for (uint8_t a = 1; a < 0xFF; a++)
            if (m_dataInfo[a].type == DynProp::oncoming){
                valForHist = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[valForHist]++;
            }

    if(statusSpeed == StatusSpeed::slowSpeed)
        for (uint8_t a = 1; a < 0xFF; a++)
            if (m_dataInfo[a].type == DynProp::oncoming ||
                    m_dataInfo[a].type == DynProp::stationary ||
                    m_dataInfo[a].type == DynProp::moving ||
                    m_dataInfo[a].type == DynProp::stationaryCandidate){
                valForHist = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[valForHist]++;
            }


    if(statusSpeed == StatusSpeed::backward)
        for (uint8_t a = 1; a < 0xFF; a++)
            if (m_dataInfo[a].type == DynProp::moving){
                valForHist = static_cast<uint8_t>(127.f + m_dataInfo[a].vRelLong * 4.0f + 0.5f);
                pHistoArray[valForHist]++;
            }

    for (uint8_t i = 4; i < 0xFF - 4; i++)
        if (pHistoArray[i] > 0)
            if (valMaxInHist < pHistoArray[i]){
                valMaxInHist = pHistoArray[i];
                indMaxValInHist = i;
            }

    // --- --- ---
    if(valMaxInHist == 0){
        speedVehicle = 0.0f;
        return 0;
    }
    // --- --- ---

    pointMax = pHistoArray[indMaxValInHist];
    point1LMax = pHistoArray[indMaxValInHist-1];
    point2LMax = pHistoArray[indMaxValInHist-2];
    point1RMax = pHistoArray[indMaxValInHist+1];
    point2RMax = pHistoArray[indMaxValInHist+2];

    valMaxInHist = point1LMax + pointMax + point1RMax;

    if (point1LMax + point1RMax)
        koeff = static_cast<float>(pointMax) / static_cast<float>(point1LMax + point1RMax);

    if (valMaxInHist > 8 && clustList.numExpectSumm > 8){
        float averSpeed4 = 0.0f;
        float valMax4 = 0.0f;

        float averSpeed = (point1LMax * (indMaxValInHist - 1) + pointMax * (indMaxValInHist) + point1RMax * (indMaxValInHist + 1))
                / static_cast<float>(point1LMax+pointMax+point1RMax);

        if (point1LMax > point1RMax){
            valMax4 = point2LMax + point1LMax + pointMax + point1RMax;
            averSpeed4 = (point2LMax * (indMaxValInHist - 2) + point1LMax * (indMaxValInHist - 1) + pointMax * (indMaxValInHist) +
                          point1RMax * (indMaxValInHist + 1)) / static_cast<float>(valMax4);
        }
        else{
            valMax4 = point1LMax + pointMax + point1RMax + point2RMax;

            averSpeed4 = (point1LMax * (indMaxValInHist - 1) + pointMax * (indMaxValInHist) + point1RMax * (indMaxValInHist + 1) +
                          point2RMax * (indMaxValInHist + 2)) / static_cast<float>(valMax4);
        }

        if (koeff > 3) averSpeedRes = averSpeed;
        else averSpeedRes = averSpeed4;

        // --- --- ---
        /*if (fabs(averSpeedKalman - 127) > 0.01f){
            averSpeedKalman = (averSpeedRes * koeffKalmanSpeed + averSpeedKalman * (1 - koeffKalmanSpeed));
            std::cout << "averSpeedKalman1: " << averSpeedKalman <<std::endl;
        }
        else{
            averSpeedKalman = averSpeedRes;
        }*/
        averSpeedKalman = averSpeedRes;

        // --- QUALITY ---
        if (indMaxValInHist >= zonaAnalyseQuality && indMaxValInHist < DATA_SIZE - zonaAnalyseQuality){
            float var3 = 0.0f;
            float summ3 = 0.0f;
            pHistoArray[indMaxValInHist - 1] += 2;
            pHistoArray[indMaxValInHist + 1] += 2;

            for (int16_t i = -zonaAnalyseQuality; i <= zonaAnalyseQuality; ++i){
                var3 += pHistoArray[indMaxValInHist + i] * i * i;
                summ3 += pHistoArray[indMaxValInHist + i];
            }
            var3 /= summ3;
            qualitySpeed = (5 * var3) / valMaxInHist;
        }
        else{
            qualitySpeed = 60.0f;
        }

        if (averQuality < 59)
            averQuality = (1 - koeffKalmanQuality) * averQuality + koeffKalmanQuality * qualitySpeed;
        else
            averQuality = qualitySpeed;

        speedVehicle = (averSpeedKalman - 127.f) * 0.25f;
        speedQuality = averQuality;
        //std::cout << "speedVehicle: " << speedVehicle <<std::endl;
    }
    else{
        speedQuality = 60.0f;
        speedVehicle = 0;
        return 0;
    }
    return 0;
}

void DisplayData::calcPath(){
    uint64_t differTime = GET_CUR_TIME_MILLI - lastRadar600Ms;
    lastRadar600Ms = GET_CUR_TIME_MILLI;
    if(differTime > (RADAR_TIME_DATA_MS + RADAR_TIME_GAP_MS))
        differTime = RADAR_TIME_DATA_MS;

    uint32_t smallDistCm = speedVehicle * 100 * (differTime/1000.0);
    radarPathCm += smallDistCm;
}

void DisplayData::showSpeedPathUI(){
    // --- SPEED ---
    QString strSpeed = "Speed: " + Converter::floatCutOff(speedVehicle, 1) + "m/s (" +
            Converter::floatCutOff(speedVehicle * 3.6f, 1) + "km/h)";
    ui->lSpeed_M_KM->setText(strSpeed);

    // --- PATH ---
    float spentPath = radarPathCm/100.0f;
    QString spentFloat = Converter::floatCutOff(spentPath, 1);
    QString strPath = "Distance: " + spentFloat + "m";
    ui->lPath_m->setText(strPath);
}

void DisplayData::slotUpdateWarningsUI(){
    QString strNoWarn = "<--- no errors --->";
    // --- temper ---
    if(dConfig->configRadar.temperatErr){
        QString errorString = "<-TEMPERATURE ERROR!->";
        ui->lWarning->setStyleSheet("background-color: red");
        ui->lWarning->text() == errorString ?
                    ui->lWarning->setText("----- ----- ----- ----- -----") :
                    ui->lWarning->setText(errorString);
    }
    // --- tempor ---
    else if(dConfig->configRadar.temporarErr){
        QString errorString = "<-TEMPORARY ERROR!->";
        ui->lWarning->setStyleSheet("background-color: red");
        ui->lWarning->text() == errorString ?
                    ui->lWarning->setText("----- ----- ----- ----- -----") :
                    ui->lWarning->setText(errorString);
    }
    // --- persist ---
    else if(dConfig->configRadar.persistErr){
        QString errorString = "<-PERSISTENT ERROR!->";
        ui->lWarning->setStyleSheet("background-color: red");
        ui->lWarning->text() == errorString ?
                    ui->lWarning->setText("----- ----- ----- ----- -----") :
                    ui->lWarning->setText(errorString);
    }
    // --- interf ---
    else if(dConfig->configRadar.interference){
        QString errorString = "<-INTERFERENCE DETECTED->";
        ui->lWarning->setStyleSheet("background-color: red");
        ui->lWarning->text() == errorString ?
                    ui->lWarning->setText("----- ----- ----- ----- -----") :
                    ui->lWarning->setText(errorString);
    }
    else{
        ui->lWarning->setStyleSheet("background-color: green");
        ui->lWarning->setText(strNoWarn);
    }
}

void DisplayData::slotZoomChanged(int val){
    QPoint oldCent(saCentr.x() * vDraw->width(), saCentr.y() * vDraw->height());

    int hei = val / aspect;
    vDraw->setFixedSize(val, hei);

    QPoint newCent(saCentr.x() * vDraw->width(), saCentr.y() * vDraw->height());
    ui->sAVisual->horizontalScrollBar()->setValue(newCent.x() - (ui->sAVisual->width() / 2));
    ui->sAVisual->verticalScrollBar()->setValue(newCent.y() - (ui->sAVisual->height() / 2));

    if(vDraw->width() >= ui->sAVisual->width())
        vDraw->sAObj.setX(vDraw->sACls.x() + ui->sAVisual->width() / 2);
    else
        vDraw->sAObj.setX(vDraw->width() / 2);
}

void DisplayData::slotSAHorizChanged(int val){
    vDraw->sACls.setX(val);
    vDraw->sAObj.setX(val + ui->sAVisual->width() / 2);

    saCentr.setX((ui->sAVisual->horizontalScrollBar()->value() + ui->sAVisual->width() / 2) / (float)vDraw->width());
}

void DisplayData::slotSAVertChanged(int val){
    vDraw->sACls.setY(val);
    vDraw->sAObj.setY(val);

    saCentr.setY((ui->sAVisual->verticalScrollBar()->value() + ui->sAVisual->height() / 2) / (float)vDraw->height());
}

void DisplayData::resizeEvent(QResizeEvent *event){
    if(countStartResize > 0){
        if(vDraw->width() >= ui->sAVisual->width())
            vDraw->sAObj.setX(vDraw->sACls.x() + ui->sAVisual->width() / 2);
    }
    else{
        vDraw->sAObj.setX(200);
        countStartResize++;
    }
}

void DisplayData::slotRadNum(int index){
    dConfig->configRadar.id = index;
    clustersFiltered.clear();
}

void DisplayData::slotInfo(bool checked){
    vDraw->isShowInfo = checked;
}

void DisplayData::slotConfigRadarCall(){
    dConfig->setWindowTitle("Radar " + QString::number(dConfig->configRadar.id));
    dConfig->show();
}
