#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cmBRadNum->addItem("Radar " + QString::number(i));

    ui->cBChsDist->setCurrentIndex(4); // 160m
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(CanLine *canLine){
    if(canLine->messId[0] == '6' && canLine->messId[2] == '0'){
        clustersAll.clear();

        numExpect = Converter::getDecData(canLine->messData, 0, 8);
        //std::cout << GET_CUR_TIME_MILLI << " | " << numExpect << std::endl;
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '1'){
        ClusterInfo cluster;

        // ID
        cluster.id = Converter::getDecData(canLine->messData, 0, 8);

        // RCS
        cluster.RCS = Converter::getDecData(canLine->messData, 56, 8);
        cluster.RCS *= 0.5f;
        cluster.RCS -= 64.0f;

        // VRelLong
        cluster.vRelLong = Converter::getDecData(canLine->messData, 32, 10);
        cluster.vRelLong *= 0.25f;
        cluster.vRelLong -= 128.0f;

        // VRelLat
        cluster.vRelLat = Converter::getDecData(canLine->messData, 42, 9);
        cluster.vRelLat *= 0.25f;
        cluster.vRelLat -= 64.0f;

        // DistLong
        cluster.distLong = Converter::getDecData(canLine->messData, 8, 13);
        cluster.distLong *= 0.2f;
        cluster.distLong -= 500.0f;

        // DistLat
        cluster.distLat = Converter::getDecData(canLine->messData, 22, 10);
        cluster.distLat *= 0.2f;
        cluster.distLat -= 102.3f;

        // Type
        uint8_t numType = Converter::getDecData(canLine->messData, 53, 3);
        cluster.type = static_cast<ClusterDynProp>(numType);

        // ---
        cluster.toOpenGLCoords();
        clustersAll.push_back(cluster);
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '2'){
        // reserved
    }

    // --- frame got ---
    if((int)clustersAll.size() == numExpect){
        applyFilters();
        // NOTE: Send to visual
        ui->wDraw->clusters = clustersFiltered;
        ui->wDraw->update();
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
    /*int minPdh0 = ui->sBPdh0Min->value();
    int maxPdh0 = ui->sBPdh0Max->value();
    int minAzmth = ui->sBAzMin->value();
    int maxAzmth = ui->sBAzMax->value();*/

    for(const auto &cl : clustersAll){
        if(cl.RCS >= minRcs && cl.RCS <= maxRcs)
            if(cl.distLong >= minDLong && cl.distLong <= maxDLong)
                if(cl.distLat >= minDLat && cl.distLat <= maxDLat)
                    if(cl.vRelLong >= minVLong && cl.vRelLong <= maxVLong)
                        if(cl.vRelLat >= minVLat && cl.vRelLat <= maxVLat)
                            if(types[static_cast<uint8_t>(cl.type)])
                                clustersFiltered.push_back(cl);
    }

    /*std::cout << "Win:" << windowTitle().toStdString() << " | Before: "
     << clustersAll.size() << " | After: " << clustersFiltered.size() << std::endl;*/
}

void DisplayData::on_cBChsDist_currentTextChanged(const QString &data){
    ui->wDraw->aspect = 100.0f / data.toFloat();
    ui->wDraw->resizeAspect();
}

void DisplayData::on_cmBRadNum_currentIndexChanged(int index){
    currRadInd = index;
    clustersFiltered.clear();
}
