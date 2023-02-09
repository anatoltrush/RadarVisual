#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent): QDialog(parent), ui(new Ui::DialogConfig){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBoxSetRadId->addItem("ID " + QString::number(i));

    on_cBSetRadQual_clicked(false);
    on_cBSetRadExt_clicked(false);
    on_cBSetRadThr_clicked(false);
    on_cBSetRadStore_clicked(false);
    on_cBSetRadRelay_clicked(false);
    on_cBSetRadDist_clicked(false);
    on_cBSetRadSort_clicked(false);
    on_cBSetRadOut_clicked(false);
    on_cBSetRadPow_clicked(false);
    on_cBSetRadId_clicked(false);
}

DialogConfig::~DialogConfig(){
    delete ui;
}

void DialogConfig::updateUI(){
    // --- write ---
    configRadar->writeStatus ? ui->rBCurrRadWriteS->setChecked(true) : ui->rBCurrRadWriteF->setChecked(true);

    // --- read ---
    configRadar->readStatus ? ui->rBCurrRadReadS->setChecked(true) : ui->rBCurrRadReadF->setChecked(true);

    // --- distance ---
    ui->lECurrRadDist->setText(QString::number(configRadar->getFarZone()));

    // --- pers error ---
    configRadar->persistErr ? ui->rBCurrRadPersErrAct->setChecked(true) : ui->rBCurrRadPersErrNo->setChecked(true);

    // --- interference ---
    configRadar->interference ? ui->rBCurrRadInter->setChecked(true) : ui->rBCurrRadNoInter->setChecked(true);

    // --- temperature ---
    configRadar->temperatErr ? ui->rBCurrRadTemperErrAct->setChecked(true) : ui->rBCurrRadTemperErrNo->setChecked(true);

    // --- temporary ---
    configRadar->temporarErr ? ui->rBCurrRadTemporErrAct->setChecked(true) : ui->rBCurrRadTemporErrNo->setChecked(true);

    // --- voltage ---
    configRadar->voltErr ? ui->rBCurrRadVoltErrAct->setChecked(true) : ui->rBCurrRadVoltErrNo->setChecked(true);

    // --- sort ---
    switch (configRadar->sortInd) {
    case 0:
        ui->rBCurrRadSortNo->setChecked(true);
        break;
    case 1:
        ui->rBCurrRadSortRange->setChecked(true);
        break;
    case 2:
        ui->rBCurrRadSortRcs->setChecked(true);
        break;
    default:
        break;
    }

    // --- power ---
    switch (configRadar->power) {
    case 0:
        ui->rBCurrRadPowStand->setChecked(true);
        break;
    case 1:
        ui->rBCurrRadPow_3dB->setChecked(true);
        break;
    case 2:
        ui->rBCurrRadPow_6dB->setChecked(true);
        break;
    case 3:
        ui->rBCurrRadPow_9dB->setChecked(true);
        break;
    default:
        break;
    }

    // --- relay ---
    configRadar->relay ? ui->rBCurrRadRelAct->setChecked(true) : ui->rBCurrRadRelIn->setChecked(true);

    // --- output type ---
    switch (configRadar->outputType) {
    case 0:
        ui->rBCurrRadOutNone->setChecked(true);
        break;
    case 1:
        ui->rBCurrRadOutObj->setChecked(true);
        break;
    case 2:
        ui->rBCurrRadOutClust->setChecked(true);
        break;
    default:
        break;
    }

    // --- send qual ---
    configRadar->sendQual ? ui->rBCurrRadQualAct->setChecked(true) : ui->rBCurrRadQualIn->setChecked(true);

    // --- send ext ---
    configRadar->sendExt ? ui->rBCurrRadExtAct->setChecked(true) : ui->rBCurrRadExtIn->setChecked(true);

    // --- motion ---
    switch (configRadar->motionRxState) {
    case 0:
        ui->rBCurrRadMotInpOk->setChecked(true);
        break;
    case 1:
        ui->rBCurrRadMotSpeedM->setChecked(true);
        break;
    case 2:
        ui->rBCurrRadMotYRM->setChecked(true);
        break;
    case 3:
        ui->rBCurrRadMotSYRM->setChecked(true);
        break;
    default:
        break;
    }

    // --- threshold ---
    configRadar->thrRcs ? ui->rBCurrRadThrHigh->setChecked(true) : ui->rBCurrRadThrStand->setChecked(true);
}

void DialogConfig::on_pBClearResStr_clicked(){
    ui->lEResStr->clear();
}


void DialogConfig::on_pBRadGenerate_clicked(){
    QString binStr(64, '0');

    // --- ID ---
    if(ui->cBSetRadId->isChecked()){
        binStr.replace(6, 1, "1");
        int cBInd = ui->cBoxSetRadId->currentIndex();
        QString binId = Converter::decToBin(QString::number(cBInd), 3);
        binStr.replace(37, 3, binId);
    }

    // --- send qual ---
    if(ui->cBSetRadQual->isChecked()){
        binStr.replace(3, 1, "1");
        if(ui->rBSetRadQualIn->isChecked())
            binStr.replace(45, 1, "0");
        else binStr.replace(45, 1, "1");
    }

    // --- send ext ---
    if(ui->cBSetRadExt->isChecked()){
        binStr.replace(2, 1, "1");
        if(ui->rBSetRadExtIn->isChecked())
            binStr.replace(44, 1, "0");
        else binStr.replace(44, 1, "1");
    }

    // --- threshold ---
    if(ui->cBSetRadThr->isChecked()){
        binStr.replace(55, 1, "1");
        if(ui->rBSetRadThrIn->isChecked())
            binStr.replace(54, 1, "0");
        else binStr.replace(54, 1, "1");
    }

    // --- store ---
    if(ui->cBSetRadStore->isChecked()){
        binStr.replace(0, 1, "1");
        if(ui->rBSetRadStoreIn->isChecked())
            binStr.replace(40, 1, "0");
        else binStr.replace(40, 1, "1");
    }

    // --- relay ---
    if(ui->cBSetRadRelay->isChecked()){
        binStr.replace(47, 1, "1");
        if(ui->rBSetRadRelayIn->isChecked())
            binStr.replace(46, 1, "0");
        else binStr.replace(46, 1, "1");
    }

    // --- distance ---
    if(ui->cBSetRadDist->isChecked()){
        QString dist = QString::number((uint16_t)(ui->sBSetRadDist->value() / resMaxDist));
        uint8_t bitLen = 10;
        QString binDist = Converter::decToBin(dist, bitLen);
        binStr.replace(7, 1, "1");
        binStr.replace(8, bitLen, binDist);
    }

    // --- sort ---
    if(ui->cBSetRadSort->isChecked()){
        binStr.replace(1, 1, "1");
        if(ui->rBSetRadSortNo->isChecked())
            binStr.replace(41, 3, "000");
        if(ui->rBSetRadSortRange->isChecked())
            binStr.replace(41, 3, "001");
        if(ui->rBSetRadSortRcs->isChecked())
            binStr.replace(41, 3, "010");
    }

    // --- output type ---
    if(ui->cBSetRadOut->isChecked()){
        binStr.replace(4, 1, "1");
        if(ui->rBSetRadOutNone->isChecked())
            binStr.replace(35, 2, "00");
        if(ui->rBSetRadOutObj->isChecked())
            binStr.replace(35, 2, "01");
        if(ui->rBSetRadOutClust->isChecked())
            binStr.replace(35, 2, "10");
    }

    // --- power ---
    if(ui->cBSetRadPow->isChecked()){
        binStr.replace(5, 1, "1");
        if(ui->rBSetRadPowStand->isChecked())
            binStr.replace(32, 3, "000");
        if(ui->rBSetRadPow_3dB->isChecked())
            binStr.replace(32, 3, "001");
        if(ui->rBSetRadPow_6dB->isChecked())
            binStr.replace(32, 3, "010");
        if(ui->rBSetRadPow_9dB->isChecked())
            binStr.replace(32, 3, "011");
    }

    QString resStr("cansend can" + QString::number(configRadar->canNum) + " 2" + QString::number(configRadar->index) + "0#");
    resStr += Converter::binToHex(binStr);

    ui->lEResStr->setText(resStr);
}

void DialogConfig::on_pBSend_clicked(){
    if(ui->lEResStr->text().isEmpty()){
        QMessageBox::information(this, "Send...", "Empty can string");
        return;
    }
    int res = system(ui->lEResStr->text().toStdString().c_str());
    if(res == 0) ui->pBSend->setStyleSheet("background-color: green");
    else{
        ui->pBSend->setStyleSheet("background-color: red");
        QMessageBox::information(this, "Send...", "Smthng wrong: " + QString::number(res));
    }
}

void DialogConfig::on_cBSetRadQual_clicked(bool checked){
    ui->rBSetRadQualIn->setEnabled(checked);
    ui->rBSetRadQualAct->setEnabled(checked);
}

void DialogConfig::on_cBSetRadExt_clicked(bool checked){
    ui->rBSetRadExtIn->setEnabled(checked);
    ui->rBSetRadExtAct->setEnabled(checked);
}

void DialogConfig::on_cBSetRadThr_clicked(bool checked){
    ui->rBSetRadThrIn->setEnabled(checked);
    ui->rBSetRadThrAct->setEnabled(checked);
}

void DialogConfig::on_cBSetRadStore_clicked(bool checked){
    ui->rBSetRadStoreIn->setEnabled(checked);
    ui->rBSetRadStoreAct->setEnabled(checked);
}

void DialogConfig::on_cBSetRadRelay_clicked(bool checked){
    ui->rBSetRadRelayIn->setEnabled(checked);
    ui->rBSetRadRelayAct->setEnabled(checked);
}

void DialogConfig::on_cBSetRadDist_clicked(bool checked){
    ui->sBSetRadDist->setEnabled(checked);
}

void DialogConfig::on_cBSetRadSort_clicked(bool checked){
    ui->rBSetRadSortNo->setEnabled(checked);
    ui->rBSetRadSortRange->setEnabled(checked);
    ui->rBSetRadSortRcs->setEnabled(checked);
}

void DialogConfig::on_cBSetRadOut_clicked(bool checked){
    ui->rBSetRadOutNone->setEnabled(checked);
    ui->rBSetRadOutClust->setEnabled(checked);
    ui->rBSetRadOutObj->setEnabled(checked);
}

void DialogConfig::on_cBSetRadPow_clicked(bool checked){
    ui->rBSetRadPowStand->setEnabled(checked);
    ui->rBSetRadPow_3dB->setEnabled(checked);
    ui->rBSetRadPow_6dB->setEnabled(checked);
    ui->rBSetRadPow_9dB->setEnabled(checked);
}

void DialogConfig::on_cBSetRadId_clicked(bool checked){
    ui->cBoxSetRadId->setEnabled(checked);
}
