#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent): QDialog(parent), ui(new Ui::DialogConfig){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBoxSetRadId->addItem("ID " + QString::number(i));

    // --- connections ---
    connect(ui->tWConfig, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    ui->tWConfig->setCurrentIndex(1);
    ui->tWConfig->setCurrentIndex(0);

    connect(ui->rBSetClObjTypeCl, SIGNAL(clicked()), this, SLOT(selectedUIClusters()));
    connect(ui->rBSetClObjTypeObj, SIGNAL(clicked()), this, SLOT(selectedUIObjects()));
    ui->rBSetClObjTypeCl->click();

    // --- radar ---
    connect(ui->pBClearResStr, SIGNAL(clicked()), this, SLOT(clearResStr()));
    connect(ui->pBGenRadConf, SIGNAL(clicked()), this, SLOT(genRadConfComm()));
    connect(ui->pBGenClObjConf, SIGNAL(clicked()), this, SLOT(genClObjConfComm()));
    connect(ui->pBGenCollState, SIGNAL(clicked()), this, SLOT(genCollState()));
    connect(ui->pBGenCollRegion, SIGNAL(clicked()), this, SLOT(genCollRegion()));
    connect(ui->pBSend, SIGNAL(clicked()), this, SLOT(send()));

    connect(ui->cBSetRadQual, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadQual(bool)));
    connect(ui->cBSetRadExt, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadExt(bool)));
    connect(ui->cBSetRadThr, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadThr(bool)));
    connect(ui->cBSetRadStore, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadStore(bool)));
    connect(ui->cBSetRadRelay, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadRelay(bool)));
    connect(ui->cBSetRadDist, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadDist(bool)));
    connect(ui->cBSetRadSort, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadSort(bool)));
    connect(ui->cBSetRadOut, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadOut(bool)));
    connect(ui->cBSetRadPow, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadPow(bool)));
    connect(ui->cBSetRadId, SIGNAL(clicked(bool)), this, SLOT(showHideSetRadId(bool)));

    emit ui->cBSetRadQual->clicked(false);
    emit ui->cBSetRadExt->clicked(false);
    emit ui->cBSetRadThr->clicked(false);
    emit ui->cBSetRadStore->clicked(false);
    emit ui->cBSetRadRelay->clicked(false);
    emit ui->cBSetRadDist->clicked(false);
    emit ui->cBSetRadSort->clicked(false);
    emit ui->cBSetRadOut->clicked(false);
    emit ui->cBSetRadPow->clicked(false);
    emit ui->cBSetRadId->clicked(false);

    // --- clusters/objects ---
    connect(ui->cBSetClObjNofObjV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjNofVal(bool)));// Nof
    connect(ui->cBSetClObjNofObjA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjNofAct(bool)));
    emit ui->cBSetClObjNofObjV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjAzimV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimVal(bool))); // Azim
    connect(ui->cBSetClObjAzimA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimAct(bool)));
    emit ui->cBSetClObjAzimV->clicked(false);

    connect(ui->cBSetClObjRcsV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjRcsVal(bool)));// Rcs
    connect(ui->cBSetClObjRcsA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjRcsAct(bool)));
    emit ui->cBSetClObjRcsV->clicked(false);

    connect(ui->cBSetClObjVOncV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVRelOncVal(bool))); // VrelOnc
    connect(ui->cBSetClObjVOncA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVRelOncAct(bool)));
    emit ui->cBSetClObjVOncV->clicked(false);

    connect(ui->cBSetClObjVDepV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVRelDepVal(bool))); // VrelDep
    connect(ui->cBSetClObjVDepA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVRelDepAct(bool)));
    emit ui->cBSetClObjVDepV->clicked(false);

    connect(ui->cBSetClObjVYLRV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVYLRVal(bool))); // VYLR
    connect(ui->cBSetClObjVYLRA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVYLRAct(bool)));
    emit ui->cBSetClObjVYLRV->clicked(false);

    connect(ui->cBSetClObjVXOncV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVXOncVal(bool))); // VXOnc
    connect(ui->cBSetClObjVXOncA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVXOncAct(bool)));
    emit ui->cBSetClObjVXOncV->clicked(false);

    connect(ui->cBSetClObjVYRLV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVYRLVal(bool))); // VYRL
    connect(ui->cBSetClObjVYRLA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVYRLAct(bool)));
    emit ui->cBSetClObjVYRLV->clicked(false);

    connect(ui->cBSetClObjVXDepV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVXDepVal(bool))); // VXDep
    connect(ui->cBSetClObjVXDepA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjVXDepAct(bool)));
    emit ui->cBSetClObjVXDepV->clicked(false);

    connect(ui->cBSetClObjLTimV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjLTimVal(bool))); // LTim
    connect(ui->cBSetClObjLTimA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjLTimAct(bool)));
    emit ui->cBSetClObjLTimV->clicked(false);

    connect(ui->cBSetClObjSzV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjSzVal(bool))); // Size
    connect(ui->cBSetClObjSzA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjSzAct(bool)));
    emit ui->cBSetClObjSzV->clicked(false);

    connect(ui->cBSetClObjProbV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjProbVal(bool))); // Prob
    connect(ui->cBSetClObjProbA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjProbAct(bool)));
    emit ui->cBSetClObjProbV->clicked(false);

    connect(ui->cBSetClObjXV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjXVal(bool))); // X
    connect(ui->cBSetClObjXA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjXAct(bool)));
    emit ui->cBSetClObjXV->clicked(false);

    connect(ui->cBSetClObjYV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjYVal(bool))); // Y
    connect(ui->cBSetClObjYA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjYAct(bool)));
    emit ui->cBSetClObjYV->clicked(false);
}

DialogConfig::~DialogConfig(){
    zmqClient.stop();
    delete ui;
}

void DialogConfig::updateConfigUI(){
    // --- ZMQ ---
    if(*inUse == InUse::zmq){
        ui->lSendZmq->show();
        ui->lESendZmq->show();
    }
    else{
        ui->lSendZmq->hide();
        ui->lESendZmq->hide();
    }

    // --- write ---
    configRadar.writeStatus ? ui->rBCurrRadWriteS->setChecked(true) : ui->rBCurrRadWriteF->setChecked(true);

    // --- read ---
    configRadar.readStatus ? ui->rBCurrRadReadS->setChecked(true) : ui->rBCurrRadReadF->setChecked(true);

    // --- distance ---
    ui->lECurrRadDist->setText(QString::number(configRadar.getFarZone()));

    // --- pers error ---
    configRadar.persistErr ? ui->rBCurrRadPersErrAct->setChecked(true) : ui->rBCurrRadPersErrNo->setChecked(true);

    // --- interference ---
    configRadar.interference ? ui->rBCurrRadInter->setChecked(true) : ui->rBCurrRadNoInter->setChecked(true);

    // --- temperature ---
    configRadar.temperatErr ? ui->rBCurrRadTemperErrAct->setChecked(true) : ui->rBCurrRadTemperErrNo->setChecked(true);

    // --- temporary ---
    configRadar.temporarErr ? ui->rBCurrRadTemporErrAct->setChecked(true) : ui->rBCurrRadTemporErrNo->setChecked(true);

    // --- voltage ---
    configRadar.voltErr ? ui->rBCurrRadVoltErrAct->setChecked(true) : ui->rBCurrRadVoltErrNo->setChecked(true);

    // --- sort ---
    switch (configRadar.sortInd) {
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
    switch (configRadar.power) {
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
    configRadar.relay ? ui->rBCurrRadRelAct->setChecked(true) : ui->rBCurrRadRelIn->setChecked(true);

    // --- output type ---
    switch (configRadar.outputType) {
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
    configRadar.sendQual ? ui->rBCurrRadQualAct->setChecked(true) : ui->rBCurrRadQualIn->setChecked(true);

    // --- send ext ---
    configRadar.sendExt ? ui->rBCurrRadExtAct->setChecked(true) : ui->rBCurrRadExtIn->setChecked(true);

    // --- motion ---
    switch (configRadar.motionRxState) {
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
    configRadar.thrRcs ? ui->rBCurrRadThrHigh->setChecked(true) : ui->rBCurrRadThrStand->setChecked(true);

    // --- filters ---
    if(is203Got){
        ui->lFiltersCl->setText(QString::number(fltClust) + " filters ON");
        ui->lFiltersObj->setText(QString::number(fltObj) + " filters ON");
    }
}

void DialogConfig::updateCollDetStateUI(){
    ui->cBCurrCollStAct->setChecked(collDetState.isActive);
    ui->lCurrCollStNof->setText(QString::number(collDetState.nofRegs));
    ui->lCurrCollStDetTim->setText(QString::number(collDetState.detectTimeSec));
    ui->lCurrCollStMeas->setText(QString::number(collDetState.measCount));
}

void DialogConfig::clearResStr(){
    ui->lEResStr->clear();
    ui->cBResStr->clear();
}

void DialogConfig::genRadConfComm(){
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
        QString dist = QString::number((uint16_t)(ui->sBSetRadDist->value() / resConfRadMaxDist));
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

    // --- cansend + bin to hex ---
    QString resStr;
    if(*inUse == InUse::can)
        resStr = ("cansend " + QString::fromStdString(deviceName) + " 2" + QString::number(configRadar.id) + "0#");
    else
        resStr = ("cansend can" + QString::number(configRadar.canNum) + " 2" + QString::number(configRadar.id) + "0#");
    resStr += Converter::binToHex(binStr);

    // --- canline for zmq line ---
    if(resStr.length() >= 40){
        QStringList listData = resStr.split(' ');
        zmqCanLine.timeStamp = GET_CUR_TIME_MICRO;
        zmqCanLine.canNum = listData[1];
        zmqCanLine.messId = listData[2].mid(0, 3);
        zmqCanLine.messData = listData[2].mid(4, 23);
        zmqCanLine.messData.remove('.');
    }
    // --- UI ---
    ui->lEResStr->setText(resStr);
}

void DialogConfig::genClObjConfComm(){
    ui->cBResStr->clear();
    QList<QString> commands;
    uint8_t bitLen = 12;

    // --- number of objs ---
    if(ui->cBSetClObjNofObjV->isChecked()){
        QString strNof(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strNof.replace(0, 1, "0") : strNof.replace(0, 1, "1"); // type
        strNof.replace(6, 1, "1"); // valid
        strNof.replace(1, 4, "0000"); // index        
        if(ui->cBSetClObjNofObjA->isChecked()){ // active
            strNof.replace(5, 1, "1"); // active
            QString nof = QString::number((uint16_t)ui->sBSetClObjNofObj->value());
            strNof.replace(28, bitLen, Converter::decToBin(nof, bitLen));
        }
        else{ // not active
            strNof.replace(5, 1, "0");
        }
        commands.append(strNof);
    }

    // --- distance ---
    if(ui->cBSetClObjDistV->isChecked()){
        QString strDist(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strDist.replace(0, 1, "0") : strDist.replace(0, 1, "1"); // type
        strDist.replace(6, 1, "1"); // valid
        strDist.replace(1, 4, "0001"); // index
        if(ui->cBSetClObjDistA->isChecked()){ // active
            strDist.replace(5, 1, "1"); // active
            QString distMin = QString::number((uint16_t)(ui->sBSetClObjDistMin->value() / resFiltDist));
            strDist.replace(12, bitLen, Converter::decToBin(distMin, bitLen));
            QString distMax = QString::number((uint16_t)(ui->sBSetClObjDistMax->value() / resFiltDist));
            strDist.replace(28, bitLen, Converter::decToBin(distMax, bitLen));
        }
        else{ // not active
            strDist.replace(5, 1, "0");
        }
        commands.append(strDist);
    }

    // --- azimuth ---
    if(ui->cBSetClObjAzimV->isChecked()){
        QString strAzim(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strAzim.replace(0, 1, "0") : strAzim.replace(0, 1, "1"); // type
        strAzim.replace(6, 1, "1"); // valid
        strAzim.replace(1, 4, "0010"); // index        
        if(ui->cBSetClObjAzimA->isChecked()){ // active
            strAzim.replace(5, 1, "1"); // active
            QString azMin = QString::number((uint16_t)((ui->sBSetClObjAzimMin->value() - offsetFiltAzim) / resFiltAzim));
            strAzim.replace(12, bitLen, Converter::decToBin(azMin, bitLen));
            QString azMax = QString::number((uint16_t)((ui->sBSetClObjAzimMax->value() - offsetFiltAzim) / resFiltAzim));
            strAzim.replace(28, bitLen, Converter::decToBin(azMax, bitLen));
        }
        else{ // not active
            strAzim.replace(5, 1, "0");
        }
        commands.append(strAzim);
    }

    // --- rcs ---
    if(ui->cBSetClObjRcsV->isChecked()){
        QString strRcs(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strRcs.replace(0, 1, "0") : strRcs.replace(0, 1, "1"); // type
        strRcs.replace(6, 1, "1"); // valid
        strRcs.replace(1, 4, "0101"); // index        
        if(ui->cBSetClObjRcsA->isChecked()){ // active
            strRcs.replace(5, 1, "1"); // active
            QString rcsMin = QString::number((uint16_t)((ui->sBSetClObjRcsMin->value() - offsetFiltRcs) / resFiltRcs));
            strRcs.replace(12, bitLen, Converter::decToBin(rcsMin, bitLen));
            QString rcsMax = QString::number((uint16_t)((ui->sBSetClObjRcsMax->value() - offsetFiltRcs) / resFiltRcs));
            strRcs.replace(28, bitLen, Converter::decToBin(rcsMax, bitLen));
        }
        else{ // not active
            strRcs.replace(5, 1, "0");
        }
        commands.append(strRcs);
    }

    // --- vrel onc ---
    if(ui->cBSetClObjVOncV->isChecked()){
        QString strVOnc(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVOnc.replace(0, 1, "0") : strVOnc.replace(0, 1, "1"); // type
        strVOnc.replace(6, 1, "1"); // valid
        strVOnc.replace(1, 4, "0011"); // index        
        if(ui->cBSetClObjVOncA->isChecked()){ // active
            strVOnc.replace(5, 1, "1"); // active
            QString vOncMin = QString::number((uint16_t)(ui->sBSetClObjVOncMin->value() / resFiltVROnc));
            strVOnc.replace(12, bitLen, Converter::decToBin(vOncMin, bitLen));
            QString vOncMax = QString::number((uint16_t)(ui->sBSetClObjVOncMax->value() / resFiltVROnc));
            strVOnc.replace(28, bitLen, Converter::decToBin(vOncMax, bitLen));
        }
        else{ // not active
            strVOnc.replace(5, 1, "0");
        }
        commands.append(strVOnc);
    }

    // --- vrel dep ---
    if(ui->cBSetClObjVDepV->isChecked()){
        QString strVDep(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVDep.replace(0, 1, "0") : strVDep.replace(0, 1, "1"); // type
        strVDep.replace(6, 1, "1"); // valid
        strVDep.replace(1, 4, "0100"); // index        
        if(ui->cBSetClObjVDepA->isChecked()){ // active
            strVDep.replace(5, 1, "1"); // active
            QString vDepMin = QString::number((uint16_t)(ui->sBSetClObjVDepMin->value() / resFiltVRDep));
            strVDep.replace(12, bitLen, Converter::decToBin(vDepMin, bitLen));
            QString vDepMax = QString::number((uint16_t)(ui->sBSetClObjVDepMax->value() / resFiltVRDep));
            strVDep.replace(28, bitLen, Converter::decToBin(vDepMax, bitLen));
        }
        else{ // not active
            strVDep.replace(5, 1, "0");
        }
        commands.append(strVDep);
    }

    // --- lifetime ---
    if(ui->cBSetClObjLTimV->isChecked()){
        QString strLTim(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strLTim.replace(0, 1, "0") : strLTim.replace(0, 1, "1"); // type
        strLTim.replace(6, 1, "1"); // valid
        strLTim.replace(1, 4, "0110"); // index
        if(ui->cBSetClObjLTimA->isChecked()){ // active
            strLTim.replace(5, 1, "1"); // active
            QString ltimMin = QString::number((uint16_t)(ui->sBSetClObjLTimMin->value() / resFiltLTim));
            strLTim.replace(12, bitLen, Converter::decToBin(ltimMin, bitLen));
            QString ltimMax = QString::number((uint16_t)(ui->sBSetClObjLTimMax->value() / resFiltLTim));
            strLTim.replace(28, bitLen, Converter::decToBin(ltimMax, bitLen));
        }
        else{ // not active
            strLTim.replace(5, 1, "0");
        }
        commands.append(strLTim);
    }

    // --- size ---
    if(ui->cBSetClObjSzV->isChecked()){
        QString strSz(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strSz.replace(0, 1, "0") : strSz.replace(0, 1, "1"); // type
        strSz.replace(6, 1, "1"); // valid
        strSz.replace(1, 4, "0111"); // index
        if(ui->cBSetClObjSzA->isChecked()){ // active
            strSz.replace(5, 1, "1"); // active
            QString szMin = QString::number((uint16_t)(ui->sBSetClObjSzMin->value() / resFiltSize));
            strSz.replace(12, bitLen, Converter::decToBin(szMin, bitLen));
            QString szMax = QString::number((uint16_t)(ui->sBSetClObjSzMax->value() / resFiltSize));
            strSz.replace(28, bitLen, Converter::decToBin(szMax, bitLen));
        }
        else{ // not active
            strSz.replace(5, 1, "0");
        }
        commands.append(strSz);
    }

    // --- probability ---
    if(ui->cBSetClObjProbV->isChecked()){
        QString strProb(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strProb.replace(0, 1, "0") : strProb.replace(0, 1, "1"); // type
        strProb.replace(6, 1, "1"); // valid
        strProb.replace(1, 4, "1000"); // index
        if(ui->cBSetClObjProbA->isChecked()){ // active
            strProb.replace(5, 1, "1"); // active
            QString probMin = QString::number((uint16_t)ui->sBSetClObjProbMin->value());
            strProb.replace(12, bitLen, Converter::decToBin(probMin, bitLen));
            QString probMax = QString::number((uint16_t)ui->sBSetClObjProbMax->value());
            strProb.replace(28, bitLen, Converter::decToBin(probMax, bitLen));
        }
        else{ // not active
            strProb.replace(5, 1, "0");
        }
        commands.append(strProb);
    }

    // --- Y ---> X!!!
    if(ui->cBSetClObjYV->isChecked()){
        QString strY(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strY.replace(0, 1, "0") : strY.replace(0, 1, "1"); // type
        strY.replace(6, 1, "1"); // valid
        strY.replace(1, 4, "1001"); // index
        if(ui->cBSetClObjYA->isChecked()){ // active
            strY.replace(5, 1, "1"); // active
            QString yMin = QString::number((uint16_t)((ui->sBSetClObjYMin->value() - offsetFiltY) / resFiltY));
            strY.replace(12, bitLen, Converter::decToBin(yMin, bitLen));
            QString yMax = QString::number((uint16_t)((ui->sBSetClObjYMax->value() - offsetFiltY) / resFiltY));
            strY.replace(28, bitLen, Converter::decToBin(yMax, bitLen));
        }
        else{ // not active
            strY.replace(5, 1, "0");
        }
        commands.append(strY);
    }

    // --- X (13) ---> Y!!!
    if(ui->cBSetClObjXV->isChecked()){
        QString strX(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strX.replace(0, 1, "0") : strX.replace(0, 1, "1"); // type
        strX.replace(6, 1, "1"); // valid
        strX.replace(1, 4, "1010"); // index
        if(ui->cBSetClObjXA->isChecked()){ // active
            strX.replace(5, 1, "1"); // active
            QString xMin = QString::number((uint16_t)((ui->sBSetClObjXMin->value() - offsetFiltX) / resFiltX));
            strX.replace(11, 13, Converter::decToBin(xMin, 13));
            QString xMax = QString::number((uint16_t)((ui->sBSetClObjXMax->value() - offsetFiltX) / resFiltX));
            strX.replace(27, 13, Converter::decToBin(xMax, 13));
        }
        else{ // not active
            strX.replace(5, 1, "0");
        }
        commands.append(strX);
    }

    // --- VYRL ---
    if(ui->cBSetClObjVYRLV->isChecked()){
        QString strVYRL(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVYRL.replace(0, 1, "0") : strVYRL.replace(0, 1, "1"); // type
        strVYRL.replace(6, 1, "1"); // valid
        strVYRL.replace(1, 4, "1011"); // index
        if(ui->cBSetClObjVYRLA->isChecked()){ // active
            strVYRL.replace(5, 1, "1"); // active
            QString vyrlMin = QString::number((uint16_t)(ui->sBSetClObjVYRLMin->value() / resFiltVYRL));
            strVYRL.replace(12, bitLen, Converter::decToBin(vyrlMin, bitLen));
            QString vyrlMax = QString::number((uint16_t)(ui->sBSetClObjVYRLMax->value() / resFiltVYRL));
            strVYRL.replace(28, bitLen, Converter::decToBin(vyrlMax, bitLen));
        }
        else{ // not active
            strVYRL.replace(5, 1, "0");
        }
        commands.append(strVYRL);
    }

    // --- VXOnc ---
    if(ui->cBSetClObjVXOncV->isChecked()){
        QString strVXOnc(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVXOnc.replace(0, 1, "0") : strVXOnc.replace(0, 1, "1"); // type
        strVXOnc.replace(6, 1, "1"); // valid
        strVXOnc.replace(1, 4, "1100"); // index
        if(ui->cBSetClObjVXOncA->isChecked()){ // active
            strVXOnc.replace(5, 1, "1"); // active
            QString vXOncMin = QString::number((uint16_t)(ui->sBSetClObjVXOncMin->value() / resFiltVXOnc));
            strVXOnc.replace(12, bitLen, Converter::decToBin(vXOncMin, bitLen));
            QString vXOncMax = QString::number((uint16_t)(ui->sBSetClObjVXOncMax->value() / resFiltVXOnc));
            strVXOnc.replace(28, bitLen, Converter::decToBin(vXOncMax, bitLen));
        }
        else{ // not active
            strVXOnc.replace(5, 1, "0");
        }
        commands.append(strVXOnc);
    }

    // --- VYLR ---
    if(ui->cBSetClObjVYLRV->isChecked()){
        QString strVYLR(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVYLR.replace(0, 1, "0") : strVYLR.replace(0, 1, "1"); // type
        strVYLR.replace(6, 1, "1"); // valid
        strVYLR.replace(1, 4, "1101"); // index
        if(ui->cBSetClObjVYLRA->isChecked()){ // active
            strVYLR.replace(5, 1, "1"); // active
            QString vylrMin = QString::number((uint16_t)(ui->sBSetClObjVYLRMin->value() / resFiltVYLR));
            strVYLR.replace(12, bitLen, Converter::decToBin(vylrMin, bitLen));
            QString vylrMax = QString::number((uint16_t)(ui->sBSetClObjVYLRMax->value() / resFiltVYLR));
            strVYLR.replace(28, bitLen, Converter::decToBin(vylrMax, bitLen));
        }
        else{ // not active
            strVYLR.replace(5, 1, "0");
        }
        commands.append(strVYLR);
    }

    // --- VXDep ---
    if(ui->cBSetClObjVXDepV->isChecked()){
        QString strVXDep(40, '0');
        ui->rBSetClObjTypeCl->isChecked() ? strVXDep.replace(0, 1, "0") : strVXDep.replace(0, 1, "1"); // type
        strVXDep.replace(6, 1, "1"); // valid
        strVXDep.replace(1, 4, "1110"); // index
        if(ui->cBSetClObjVXDepA->isChecked()){ // active
            strVXDep.replace(5, 1, "1"); // active
            QString vXDepMin = QString::number((uint16_t)(ui->sBSetClObjVXDepMin->value() / resFiltVXDep));
            strVXDep.replace(12, bitLen, Converter::decToBin(vXDepMin, bitLen));
            QString vXDepMax = QString::number((uint16_t)(ui->sBSetClObjVXDepMax->value() / resFiltVXDep));
            strVXDep.replace(28, bitLen, Converter::decToBin(vXDepMax, bitLen));
        }
        else{ // not active
            strVXDep.replace(5, 1, "0");
        }
        commands.append(strVXDep);
    }

    // --- cansend + bin to hex ---
    for(auto& comm: commands){
        QString resStr;
        if(*inUse == InUse::can)
            resStr = ("cansend " + QString::fromStdString(deviceName) + " 2" + QString::number(configRadar.id) + "2#");
        else
            resStr = ("cansend can" + QString::number(configRadar.canNum) + " 2" + QString::number(configRadar.id) + "2#");
        resStr += Converter::binToHex(comm);
        comm = resStr;
    }

    // --- canlines for zmq line ---
    zmqCanLines.clear();
    for(const auto& comm: commands){
        QStringList listData = comm.split(' ');
        CanLine temp;
        temp.timeStamp = GET_CUR_TIME_MICRO;
        temp.canNum = listData[1];
        temp.messId = listData[2].mid(0, 3);
        temp.messData = listData[2].mid(4, 23);
        temp.messData.remove('.');
        zmqCanLines.push_back(temp);
    }

    // --- UI ---
    ui->cBResStr->addItems(commands);
}

void DialogConfig::genCollState(){
    QString collStatStr(16, '0');

    // --- cansend + bin to hex ---
    QString resStr;
    if(*inUse == InUse::can)
        resStr = ("cansend " + QString::fromStdString(deviceName) + " 4" + QString::number(configRadar.id) + "0#");
    else
        resStr = ("cansend can" + QString::number(configRadar.canNum) + " 4" + QString::number(configRadar.id) + "0#");
    resStr += Converter::binToHex(collStatStr);

    // --- canline for zmq line ---

    // --- UI ---
    ui->lEResStr->setText(resStr);
}

void DialogConfig::genCollRegion(){
    QString collRegStr(64, '0');

    // --- cansend + bin to hex ---
    QString resStr;
    if(*inUse == InUse::can)
        resStr = ("cansend " + QString::fromStdString(deviceName) + " 4" + QString::number(configRadar.id) + "1#");
    else
        resStr = ("cansend can" + QString::number(configRadar.canNum) + " 4" + QString::number(configRadar.id) + "1#");
    resStr += Converter::binToHex(collRegStr);

    // --- canline for zmq line ---

    // --- UI ---
    ui->lEResStr->setText(resStr);
}

void DialogConfig::send(){
    if(ui->tWConfig->currentIndex() == 0 || ui->tWConfig->currentIndex() == 2)
        sendOne();
    else sendMulti();
}

void DialogConfig::sendOne(){
    if(ui->lEResStr->text().isEmpty()){
        QMessageBox::information(this, "Send...", "Empty can string");
        return;
    }
    // --- --- ---
    switch (*inUse) {
    case InUse::nothing:    // --- NTHNG ---
        QMessageBox::information(this, "Send...", "Nothing started");
        break;
    case InUse::can:{       // --- CAN ---
#ifdef __WIN32
        int res = emit signalCanSend(ui->lEResStr->text());
#else
        int res = system(ui->lEResStr->text().toStdString().c_str());
#endif
        if(res == 0) ui->pBSend->setStyleSheet("background-color: green");
        else{
            ui->pBSend->setStyleSheet("background-color: red");
            QMessageBox::information(this, "Send via CAN...", "Smthng wrong: " + QString::number(res));
        }
        break;
    }
    case InUse::zmq:{       // --- ZMQ ---
        if(ui->lESendZmq->text().isEmpty()){
            QMessageBox::information(this, "Send via ZMQ", "Empty ZMQ address");
            return;
        }
        else{
            zmqClient.stop();
            zmqAddrSend = ui->lESendZmq->text();
            zmqClient.configure(zmqAddrSend.toStdString());
            std::string whatStart;
            bool isCliStarted = zmqClient.start(whatStart);
            if(isCliStarted){
                zmq::message_t messToCan;
                MessageId idToCan;
                idToCan._time = GET_CUR_TIME_MICRO;
                idToCan._msg_num = msg_num++;
                idToCan._msg_src = configRadar.id;
                canfd_frame canFrame;

                Converter::getCanFdFromCanLine(canFrame, zmqCanLine);
                Converter::getZmqFromCanFd(messToCan, canFrame, idToCan);
                // ---
                std::string whatSend;
                bool isSent = zmqClient.send(messToCan, whatSend);
                if(isSent){
                    zmq::message_t rcv;
                    std::string whatRcv;
                    bool isRcvd = zmqClient.receive(&rcv, whatRcv, 10);
                    if(isRcvd){
                        ui->lSendZmq->setStyleSheet("background-color: green");
                    }
                    else{
                        ui->lSendZmq->setStyleSheet("background-color: red");
                        QMessageBox::information(this, "Send via ZMQ", "No response received:\n" +
                                                 QString::fromStdString(whatRcv));
                    }
                }
                else{
                    ui->lSendZmq->setStyleSheet("background-color: red");
                    QMessageBox::information(this, "Send via ZMQ", "Can't send ZMQ:\n" + QString::fromStdString(whatSend) +
                                             "\n(Check ip & port and restart application)");
                }
            }
            else{
                ui->lSendZmq->setStyleSheet("background-color: red");
                QMessageBox::information(this, "Send via ZMQ", "Can't start ZMQ:\n" + QString::fromStdString(whatStart));
            }
        }
        break;
    }
    case InUse::file:       // --- FILE ---
        QMessageBox::information(this, "Send...", "What are you going to send to the file? :-)");
        break;
    default:
        break;
    }
}

void DialogConfig::sendMulti(){
    if(ui->cBResStr->count() == 0){
        QMessageBox::information(this, "Send...", "Empty can list");
        return;
    }
    // --- --- ---
    switch (*inUse) {
    case InUse::nothing:    // --- NTHNG ---
        QMessageBox::information(this, "Send...", "Nothing started");
        break;
    case InUse::can:{       // --- CAN ---
        for (int i = 0; i < ui->cBResStr->count(); i++) {
#ifdef __WIN32
            int res = emit signalCanSend(ui->cBResStr->itemText(i));
#else
            int res = system(ui->cBResStr->itemText(i).toStdString().c_str());
#endif
            if(res != 0){
                ui->pBSend->setStyleSheet("background-color: red");
                QMessageBox::information(this, "Send via CAN...", "Smthng wrong: " + QString::number(res));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(delaySendMsg));
        }
        ui->pBSend->setStyleSheet("background-color: green");
        break;
    }
    case InUse::zmq:{       // --- ZMQ ---
        if(ui->lESendZmq->text().isEmpty()){
            QMessageBox::information(this, "Send via ZMQ", "Empty ZMQ address");
            return;
        }
        else{
            zmqClient.stop();
            zmqAddrSend = ui->lESendZmq->text();
            zmqClient.configure(zmqAddrSend.toStdString());
            std::string whatStart;
            bool isCliStarted = zmqClient.start(whatStart);
            if(isCliStarted){
                for (const auto &zmqCL : zmqCanLines) {
                    zmq::message_t messToCan;
                    MessageId idToCan;
                    idToCan._time = GET_CUR_TIME_MICRO;
                    idToCan._msg_num = msg_num++;
                    idToCan._msg_src = configRadar.id;
                    canfd_frame canFrame;

                    Converter::getCanFdFromCanLine(canFrame, zmqCL);
                    Converter::getZmqFromCanFd(messToCan, canFrame, idToCan);
                    // ---
                    std::string whatSend;
                    bool isSent = zmqClient.send(messToCan, whatSend);
                    if(isSent){
                        zmq::message_t rcv;
                        std::string whatRcv;
                        bool isRcvd = zmqClient.receive(&rcv, whatRcv, 10);
                        if(isRcvd){
                            ui->lSendZmq->setStyleSheet("background-color: green");
                        }
                        else{
                            ui->lSendZmq->setStyleSheet("background-color: red");
                            QMessageBox::information(this, "Send via ZMQ", "No response received:\n" +
                                                     QString::fromStdString(whatRcv));
                            break;
                        }
                    }
                    else{
                        ui->lSendZmq->setStyleSheet("background-color: red");
                        QMessageBox::information(this, "Send via ZMQ", "Can't send ZMQ:\n" + QString::fromStdString(whatSend) +
                                                 "\n(Check ip & port and restart application)");
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(delaySendMsg));
                }
            }
            else{
                ui->lSendZmq->setStyleSheet("background-color: red");
                QMessageBox::information(this, "Send via ZMQ", "Can't start ZMQ:\n" + QString::fromStdString(whatStart));
            }
        }
        break;
    }
    case InUse::file:       // --- FILE ---
        QMessageBox::information(this, "Send...", "What are you going to send to the file? :-)");
        break;
    default:
        break;
    }
}

void DialogConfig::showHideSetRadQual(bool checked){
    ui->rBSetRadQualIn->setEnabled(checked);
    ui->rBSetRadQualAct->setEnabled(checked);
}

void DialogConfig::showHideSetRadExt(bool checked){
    ui->rBSetRadExtIn->setEnabled(checked);
    ui->rBSetRadExtAct->setEnabled(checked);
}

void DialogConfig::showHideSetRadThr(bool checked){
    ui->rBSetRadThrIn->setEnabled(checked);
    ui->rBSetRadThrAct->setEnabled(checked);
}

void DialogConfig::showHideSetRadStore(bool checked){
    ui->rBSetRadStoreIn->setEnabled(checked);
    ui->rBSetRadStoreAct->setEnabled(checked);
}

void DialogConfig::showHideSetRadRelay(bool checked){
    ui->rBSetRadRelayIn->setEnabled(checked);
    ui->rBSetRadRelayAct->setEnabled(checked);
}

void DialogConfig::showHideSetRadDist(bool checked){
    ui->sBSetRadDist->setEnabled(checked);
}

void DialogConfig::showHideSetRadSort(bool checked){
    ui->rBSetRadSortNo->setEnabled(checked);
    ui->rBSetRadSortRange->setEnabled(checked);
    ui->rBSetRadSortRcs->setEnabled(checked);
}

void DialogConfig::showHideSetRadOut(bool checked){
    ui->rBSetRadOutNone->setEnabled(checked);
    ui->rBSetRadOutClust->setEnabled(checked);
    ui->rBSetRadOutObj->setEnabled(checked);
}

void DialogConfig::showHideSetRadPow(bool checked){
    ui->rBSetRadPowStand->setEnabled(checked);
    ui->rBSetRadPow_3dB->setEnabled(checked);
    ui->rBSetRadPow_6dB->setEnabled(checked);
    ui->rBSetRadPow_9dB->setEnabled(checked);
}

void DialogConfig::showHideSetRadId(bool checked){
    ui->cBoxSetRadId->setEnabled(checked);
}

void DialogConfig::tabChanged(int index){
    if(index == 0 || index == 2){
        ui->lEResStr->show();
        ui->cBResStr->hide();
    }
    else{
        ui->cBResStr->show();
        ui->lEResStr->hide();
    }
}

void DialogConfig::selectedUIClusters(){
    ui->gBSetClObjX->setEnabled(false);
    ui->gBSetClObjY->setEnabled(false);
    ui->gBSetClObjVXOnc->setEnabled(false);
    ui->gBSetClObjVXDep->setEnabled(false);
    ui->gBSetClObjVYRL->setEnabled(false);
    ui->gBSetClObjVYLR->setEnabled(false);
    ui->gBSetClObjLTim->setEnabled(false);
    ui->gBSetClObjSz->setEnabled(false);
    ui->gBSetClObjProb->setEnabled(false);

    ui->cBSetClObjXV->setChecked(false);
    ui->cBSetClObjYV->setChecked(false);
    ui->cBSetClObjVXOncV->setChecked(false);
    ui->cBSetClObjVXDepV->setChecked(false);
    ui->cBSetClObjVYRLV->setChecked(false);
    ui->cBSetClObjVYLRV->setChecked(false);
    ui->cBSetClObjLTimV->setChecked(false);
    ui->cBSetClObjSzV->setChecked(false);
    ui->cBSetClObjProbV->setChecked(false);

    emit ui->cBSetClObjXV->clicked(false);
    emit ui->cBSetClObjYV->clicked(false);
    emit ui->cBSetClObjVXOncV->clicked(false);
    emit ui->cBSetClObjVXDepV->clicked(false);
    emit ui->cBSetClObjVYRLV->clicked(false);
    emit ui->cBSetClObjVYLRV->clicked(false);
    emit ui->cBSetClObjLTimV->clicked(false);
    emit ui->cBSetClObjSzV->clicked(false);
    emit ui->cBSetClObjProbV->clicked(false);
}

void DialogConfig::selectedUIObjects(){
    ui->gBSetClObjVOnc->setEnabled(true);
    ui->gBSetClObjVDep->setEnabled(true);
    ui->gBSetClObjX->setEnabled(true);
    ui->gBSetClObjY->setEnabled(true);
    ui->gBSetClObjVXOnc->setEnabled(true);
    ui->gBSetClObjVXDep->setEnabled(true);
    ui->gBSetClObjVYRL->setEnabled(true);
    ui->gBSetClObjVYLR->setEnabled(true);
    ui->gBSetClObjLTim->setEnabled(true);
    ui->gBSetClObjSz->setEnabled(true);
    ui->gBSetClObjProb->setEnabled(true);
}

void DialogConfig::showHideSetClObjNofVal(bool checked){
    ui->cBSetClObjNofObjA->setEnabled(checked);
    ui->sBSetClObjNofObj->setEnabled(ui->cBSetClObjNofObjA->isEnabled() && ui->cBSetClObjNofObjA->isChecked());
}

void DialogConfig::showHideSetClObjNofAct(bool checked){
    ui->sBSetClObjNofObj->setEnabled(checked);
}

void DialogConfig::showHideSetClObjDistVal(bool checked){
    ui->cBSetClObjDistA->setEnabled(checked);
    ui->sBSetClObjDistMin->setEnabled(ui->cBSetClObjDistA->isEnabled() && ui->cBSetClObjDistA->isChecked());
    ui->sBSetClObjDistMax->setEnabled(ui->cBSetClObjDistA->isEnabled() && ui->cBSetClObjDistA->isChecked());
}

void DialogConfig::showHideSetClObjDistAct(bool checked){
    ui->sBSetClObjDistMin->setEnabled(checked);
    ui->sBSetClObjDistMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjAzimVal(bool checked){
    ui->cBSetClObjAzimA->setEnabled(checked);
    ui->sBSetClObjAzimMin->setEnabled(ui->cBSetClObjAzimA->isEnabled() && ui->cBSetClObjAzimA->isChecked());
    ui->sBSetClObjAzimMax->setEnabled(ui->cBSetClObjAzimA->isEnabled() && ui->cBSetClObjAzimA->isChecked());
}

void DialogConfig::showHideSetClObjAzimAct(bool checked){
    ui->sBSetClObjAzimMin->setEnabled(checked);
    ui->sBSetClObjAzimMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjRcsVal(bool checked){
    ui->cBSetClObjRcsA->setEnabled(checked);
    ui->sBSetClObjRcsMin->setEnabled(ui->cBSetClObjRcsA->isEnabled() && ui->cBSetClObjRcsA->isChecked());
    ui->sBSetClObjRcsMax->setEnabled(ui->cBSetClObjRcsA->isEnabled() && ui->cBSetClObjRcsA->isChecked());
}

void DialogConfig::showHideSetClObjRcsAct(bool checked){
    ui->sBSetClObjRcsMin->setEnabled(checked);
    ui->sBSetClObjRcsMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVRelOncVal(bool checked){
    ui->cBSetClObjVOncA->setEnabled(checked);
    ui->sBSetClObjVOncMin->setEnabled(ui->cBSetClObjVOncA->isEnabled() && ui->cBSetClObjVOncA->isChecked());
    ui->sBSetClObjVOncMax->setEnabled(ui->cBSetClObjVOncA->isEnabled() && ui->cBSetClObjVOncA->isChecked());
}

void DialogConfig::showHideSetClObjVRelOncAct(bool checked){
    ui->sBSetClObjVOncMin->setEnabled(checked);
    ui->sBSetClObjVOncMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVRelDepVal(bool checked){
    ui->cBSetClObjVDepA->setEnabled(checked);
    ui->sBSetClObjVDepMin->setEnabled(ui->cBSetClObjVDepA->isEnabled() && ui->cBSetClObjVDepA->isChecked());
    ui->sBSetClObjVDepMax->setEnabled(ui->cBSetClObjVDepA->isEnabled() && ui->cBSetClObjVDepA->isChecked());
}

void DialogConfig::showHideSetClObjVRelDepAct(bool checked){
    ui->sBSetClObjVDepMin->setEnabled(checked);
    ui->sBSetClObjVDepMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVYLRVal(bool checked){
    ui->cBSetClObjVYLRA->setEnabled(checked);
    ui->sBSetClObjVYLRMin->setEnabled(ui->cBSetClObjVYLRA->isEnabled() && ui->cBSetClObjVYLRA->isChecked());
    ui->sBSetClObjVYLRMax->setEnabled(ui->cBSetClObjVYLRA->isEnabled() && ui->cBSetClObjVYLRA->isChecked());
}

void DialogConfig::showHideSetClObjVYLRAct(bool checked){
    ui->sBSetClObjVYLRMin->setEnabled(checked);
    ui->sBSetClObjVYLRMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVXOncVal(bool checked){
    ui->cBSetClObjVXOncA->setEnabled(checked);
    ui->sBSetClObjVXOncMin->setEnabled(ui->cBSetClObjVXOncA->isEnabled() && ui->cBSetClObjVXOncA->isChecked());
    ui->sBSetClObjVXOncMax->setEnabled(ui->cBSetClObjVXOncA->isEnabled() && ui->cBSetClObjVXOncA->isChecked());
}

void DialogConfig::showHideSetClObjVXOncAct(bool checked){
    ui->sBSetClObjVXOncMin->setEnabled(checked);
    ui->sBSetClObjVXOncMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVYRLVal(bool checked){
    ui->cBSetClObjVYRLA->setEnabled(checked);
    ui->sBSetClObjVYRLMin->setEnabled(ui->cBSetClObjVYRLA->isEnabled() && ui->cBSetClObjVYRLA->isChecked());
    ui->sBSetClObjVYRLMax->setEnabled(ui->cBSetClObjVYRLA->isEnabled() && ui->cBSetClObjVYRLA->isChecked());
}

void DialogConfig::showHideSetClObjVYRLAct(bool checked){
    ui->sBSetClObjVYRLMin->setEnabled(checked);
    ui->sBSetClObjVYRLMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVXDepVal(bool checked){
    ui->cBSetClObjVXDepA->setEnabled(checked);
    ui->sBSetClObjVXDepMin->setEnabled(ui->cBSetClObjVXDepA->isEnabled() && ui->cBSetClObjVXDepA->isChecked());
    ui->sBSetClObjVXDepMax->setEnabled(ui->cBSetClObjVXDepA->isEnabled() && ui->cBSetClObjVXDepA->isChecked());
}

void DialogConfig::showHideSetClObjVXDepAct(bool checked){
    ui->sBSetClObjVXDepMin->setEnabled(checked);
    ui->sBSetClObjVXDepMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjLTimVal(bool checked){
    ui->cBSetClObjLTimA->setEnabled(checked);
    ui->sBSetClObjLTimMin->setEnabled(ui->cBSetClObjLTimA->isEnabled() && ui->cBSetClObjLTimA->isChecked());
    ui->sBSetClObjLTimMax->setEnabled(ui->cBSetClObjLTimA->isEnabled() && ui->cBSetClObjLTimA->isChecked());
}

void DialogConfig::showHideSetClObjLTimAct(bool checked){
    ui->sBSetClObjLTimMin->setEnabled(checked);
    ui->sBSetClObjLTimMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjSzVal(bool checked){
    ui->cBSetClObjSzA->setEnabled(checked);
    ui->sBSetClObjSzMin->setEnabled(ui->cBSetClObjSzA->isEnabled() && ui->cBSetClObjSzA->isChecked());
    ui->sBSetClObjSzMax->setEnabled(ui->cBSetClObjSzA->isEnabled() && ui->cBSetClObjSzA->isChecked());
}

void DialogConfig::showHideSetClObjSzAct(bool checked){
    ui->sBSetClObjSzMin->setEnabled(checked);
    ui->sBSetClObjSzMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjProbVal(bool checked){
    ui->cBSetClObjProbA->setEnabled(checked);
    ui->sBSetClObjProbMin->setEnabled(ui->cBSetClObjProbA->isEnabled() && ui->cBSetClObjProbA->isChecked());
    ui->sBSetClObjProbMax->setEnabled(ui->cBSetClObjProbA->isEnabled() && ui->cBSetClObjProbA->isChecked());
}

void DialogConfig::showHideSetClObjProbAct(bool checked){
    ui->sBSetClObjProbMin->setEnabled(checked);
    ui->sBSetClObjProbMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjXVal(bool checked){
    ui->cBSetClObjXA->setEnabled(checked);
    ui->sBSetClObjXMin->setEnabled(ui->cBSetClObjXA->isEnabled() && ui->cBSetClObjXA->isChecked());
    ui->sBSetClObjXMax->setEnabled(ui->cBSetClObjXA->isEnabled() && ui->cBSetClObjXA->isChecked());
}

void DialogConfig::showHideSetClObjXAct(bool checked){
    ui->sBSetClObjXMin->setEnabled(checked);
    ui->sBSetClObjXMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjYVal(bool checked){
    ui->cBSetClObjYA->setEnabled(checked);
    ui->sBSetClObjYMin->setEnabled(ui->cBSetClObjYA->isEnabled() && ui->cBSetClObjYA->isChecked());
    ui->sBSetClObjYMax->setEnabled(ui->cBSetClObjYA->isEnabled() && ui->cBSetClObjYA->isChecked());
}

void DialogConfig::showHideSetClObjYAct(bool checked){
    ui->sBSetClObjYMin->setEnabled(checked);
    ui->sBSetClObjYMax->setEnabled(checked);
}
