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
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(CanLine *canLine){
    if(canLine->messId[0] == '6' && canLine->messId[2] == '0'){
        clustersAll.clear();

        numExpectNear = Converter::getDecData(canLine->messData, 0, 8);
        numExpectFar = Converter::getDecData(canLine->messData, 8, 8);
        numExpectSumm = numExpectNear + numExpectFar;
        measCount = Converter::getDecData(canLine->messData, 16, 16);
        //std::cout << GET_CUR_TIME_MILLI << " | " << (int)numExpectSumm << std::endl;
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '1'){
        ClusterInfo cluster;

        // ID
        cluster.id = Converter::getDecData(canLine->messData, 0, 8);

        // RCS
        cluster.RCS = Converter::getDecData(canLine->messData, 56, 8);
        cluster.RCS *= resRCS;
        cluster.RCS += offsetRCS;

        // VRelLong
        cluster.vRelLong = Converter::getDecData(canLine->messData, 32, 10);
        cluster.vRelLong *= resVRelLong;
        cluster.vRelLong += offsetVRelLong;

        // VRelLat
        cluster.vRelLat = Converter::getDecData(canLine->messData, 42, 9);
        cluster.vRelLat *= resVRelLat;
        cluster.vRelLat += offsetVRelLat;

        // DistLong
        cluster.distLong = Converter::getDecData(canLine->messData, 8, 13);
        cluster.distLong *= resDistLong;
        cluster.distLong += offsetDistLong;

        // DistLat
        cluster.distLat = Converter::getDecData(canLine->messData, 22, 10);
        cluster.distLat *= resDistLat;
        cluster.distLat += offsetDistLat;
        cluster.distLat = -cluster.distLat;// NOTE: left/right?

        // Type
        uint8_t numType = Converter::getDecData(canLine->messData, 53, 3);
        cluster.type = static_cast<ClusterDynProp>(numType);

        // Azimuth
        cluster.clacAzimuth();

        // ---
        clustersAll.push_back(cluster);
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '2'){
        uint8_t idCl = Converter::getDecData(canLine->messData, 0, 8);
        uint8_t pDh0Cl = Converter::getDecData(canLine->messData, 29, 3);
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

    // --- frame got ---
    if((int)clustersAll.size() == numExpectSumm){
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

void DisplayData::on_cBChsDist_currentTextChanged(const QString &data){
    ui->wDraw->aspect = 100.0f / data.toFloat();
    ui->wDraw->resizeAspect();
}

void DisplayData::on_cmBRadNum_currentIndexChanged(int index){
    currRadInd = index;
    clustersFiltered.clear();
}

void DisplayData::on_cBInfo_clicked(bool checked){
    ui->wDraw->isShowInfo = checked;
}
