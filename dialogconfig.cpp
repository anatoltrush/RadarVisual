#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent): QDialog(parent), ui(new Ui::DialogConfig){
    ui->setupUi(this);
}

DialogConfig::~DialogConfig(){
    delete ui;
}

void DialogConfig::updateUI(){
    // --- write ---
    configRadar->writeStatus ? ui->rBCurrRadWriteS->setChecked(true) : ui->rBCurrRadWriteF->setChecked(true);

    // --- read ---
    configRadar->readStatus ? ui->rBCurrRadReadS->setChecked(true) : ui->rBCurrRadReadF->setChecked(true);

    // --- dist ---
    ui->lECurrRadDist->setText(QString::number(configRadar->getFarZone()));

    // --- pers error ---
    configRadar->persistErr ? ui->rBCurrRadPersErrAct->setChecked(true) : ui->rBCurrRadPersErrNo->setChecked(true);

    // --- interference ---
    configRadar->interference ? ui->rBCurrRadInter->setChecked(true) : ui->rBCurrRadNoI->setChecked(true);

    // --- temperature ---
    configRadar->temperatErr ? ui->rBCurrRadTemperErrAct->setChecked(true) : ui->rBCurrRadTemperErrNo->setChecked(true);

    // --- temporary ---
    configRadar->temporarErr ? ui->rBCurrRadTemporErrAct->setChecked(true) : ui->rBCurrRadTemporErrNo->setChecked(true);

    // --- voltage ---
    configRadar->voltErr ? ui->rBCurrRadVoltErrAct->setChecked(true) : ui->rBCurrRadVoltErrNo->setChecked(true);
}

void DialogConfig::on_pBClearResStr_clicked(){
    ui->lEResStr->clear();
}

