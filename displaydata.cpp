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
        // --- parse ---
        QString hexNum(canLine->messData.data(), 2);
        bool ok = false;
        numExpect = hexNum.toInt(&ok, 16);
        // int a = 5;
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '1'){
        //canFrame.generalInfo.push_back(canLine);
        // --- parse ---
        /*bool ok = false;
        QString binAll = QString::number(canLine->messData.toLongLong(&ok, 16), 2);
        while (binAll.length() < canLine->messData.length() * 4)
            binAll.prepend("0");*/

        /*QString binNum(binAll.data(), 8);
        int decimal = binNum.toInt(&ok, 2);*/

        int a = 5;
    }
    if(canLine->messId[0] == '7' && canLine->messId[2] == '2'){
        //canFrame.qualityInfo.push_back(canLine);
    }

    // --- frame got ---
    if((int)canFrame.generalInfo.size() == numExpect){
        // do smthng
    }
}
