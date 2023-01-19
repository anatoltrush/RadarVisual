#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cmBRadNum->addItem("Radar " + QString::number(i));
}

DisplayData::~DisplayData(){
    delete ui;
}

void DisplayData::receiveCanLine(CanLine *canLine){
    if(canLine->messId[0] == '6' && canLine->messId[2] == '0'){
        canFrame.clear();
        canFrame.header = canLine;

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
        canFrame.generalInfo.push_back(cluster);
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '2'){
        // reserved
    }

    // --- frame got ---
    if((int)canFrame.generalInfo.size() == numExpect){
        // --- draw canFrame ---
        ui->displayWidget->clusters = &canFrame.generalInfo;
    }
}
