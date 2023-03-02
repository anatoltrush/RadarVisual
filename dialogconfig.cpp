#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent): QDialog(parent), ui(new Ui::DialogConfig){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBoxSetRadId->addItem("ID " + QString::number(i));

    // --- connections ---
    connect(ui->tWConfig, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    ui->tWConfig->setCurrentIndex(0);

    connect(ui->rBSetClObjTypeCl, SIGNAL(clicked()), this, SLOT(updateUIClusters()));
    connect(ui->rBSetClObjTypeObj, SIGNAL(clicked()), this, SLOT(updateUIObjects()));
    ui->rBSetClObjTypeCl->click();

    // --- radar ---
    connect(ui->pBClearResStr, SIGNAL(clicked()), this, SLOT(clearResStr()));
    connect(ui->pBGenRadConf, SIGNAL(clicked()), this, SLOT(genRadConfComm()));
    connect(ui->pBGenClObjConf, SIGNAL(clicked()), this, SLOT(genClObjConfComm()));
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

    /* VREL 4 connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);*/

    /* L S B connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);

    connect(ui->cBSetClObjDistV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistVal(bool))); // Dist
    connect(ui->cBSetClObjDistA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjDistAct(bool)));
    emit ui->cBSetClObjDistV->clicked(false);*/

    /* X, Y connect(ui->cBSetClObjAzimV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimVal(bool))); // Azim
    connect(ui->cBSetClObjAzimA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimAct(bool)));
    emit ui->cBSetClObjAzimV->clicked(false);

    connect(ui->cBSetClObjAzimV, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimVal(bool))); // Azim
    connect(ui->cBSetClObjAzimA, SIGNAL(clicked(bool)), this, SLOT(showHideSetClObjAzimAct(bool)));
    emit ui->cBSetClObjAzimV->clicked(false);*/
}

DialogConfig::~DialogConfig(){
    zmqClient.stop();

    delete ui;
}

void DialogConfig::updateUI(){
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

    // --- filters ---
    if(is203Got){
        ui->lFiltersCl->setText(QString::number(fltClust) + " filters ON");
        ui->lFiltersObj->setText(QString::number(fltObj) + " filters ON");
    }
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
        resStr = ("cansend " + QString::fromStdString(deviceName) + " 2" + QString::number(configRadar->id) + "0#");
    else
        resStr = ("cansend can" + QString::number(configRadar->canNum) + " 2" + QString::number(configRadar->id) + "0#");
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
        strNof.replace(1, 4, "0000"); // index
        strNof.replace(6, 1, "1"); // valid
        if(ui->cBSetClObjNofObjA->isChecked()){ // active
            strNof.replace(5, 1, "1"); // active
            QString nof = QString::number((uint16_t)(ui->sBSetClObjNofObj->value()));
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
        strDist.replace(1, 4, "0001"); // index
        strDist.replace(6, 1, "1"); // valid
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
        strAzim.replace(1, 4, "0010"); // index
        strAzim.replace(6, 1, "1"); // valid
        if(ui->cBSetClObjAzimA->isChecked()){ // active
            strAzim.replace(5, 1, "1"); // active  // NOTE: azimuth objects left/right?
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
        strRcs.replace(1, 4, "0101"); // index
        strRcs.replace(6, 1, "1"); // valid
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
        strVOnc.replace(1, 4, "0011"); // index
        strVOnc.replace(6, 1, "1"); // valid
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
        strVDep.replace(1, 4, "0100"); // index
        strVDep.replace(6, 1, "1"); // valid
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

    // --- cansend + bin to hex ---
    for(auto& comm: commands){
        QString resStr;
        if(*inUse == InUse::can)
            resStr = ("cansend " + QString::fromStdString(deviceName) + " 2" + QString::number(configRadar->id) + "2#");
        else
            resStr = ("cansend can" + QString::number(configRadar->canNum) + " 2" + QString::number(configRadar->id) + "2#");
        resStr += Converter::binToHex(comm);
        comm = resStr;
    }

    // TODO:--- canlines for zmq line ---

    // --- UI ---
    ui->cBResStr->addItems(commands);
}

void DialogConfig::send(){
    ui->tWConfig->currentIndex() == 0 ? sendOne() : sendMulti();
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
        int res = system(ui->lEResStr->text().toStdString().c_str());
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
                idToCan._msg_src = configRadar->id;
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
                        QMessageBox::information(this, "Send via ZMQ", "Can't send ZMQ:\n" + QString::fromStdString(whatRcv));
                    }
                }
                else{
                    ui->lSendZmq->setStyleSheet("background-color: red");
                    QMessageBox::information(this, "Send via ZMQ", "No response received:\n"
+ QString::fromStdString(whatSend) + "\n(Check ip & port and restart application)");
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
            int res = system(ui->cBResStr->itemText(i).toStdString().c_str());
            if(res != 0){
                ui->pBSend->setStyleSheet("background-color: red");
                QMessageBox::information(this, "Send via CAN...", "Smthng wrong: " + QString::number(res));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(delayCanMs));
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
                // TODO: zmq implement
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
    if(index == 0){
        ui->lEResStr->show();
        ui->cBResStr->hide();
    }
    else{
        ui->cBResStr->show();
        ui->lEResStr->hide();
    }
}

void DialogConfig::updateUIClusters(){
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

void DialogConfig::updateUIObjects(){
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
    ui->sBSetClObjDistMin->setEnabled(ui->cBSetClObjDistA->isEnabled()&& ui->cBSetClObjDistA->isChecked());
    ui->sBSetClObjDistMax->setEnabled(ui->cBSetClObjDistA->isEnabled() && ui->cBSetClObjDistA->isChecked());
}

void DialogConfig::showHideSetClObjDistAct(bool checked){
    ui->sBSetClObjDistMin->setEnabled(checked);
    ui->sBSetClObjDistMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjAzimVal(bool checked){
    ui->cBSetClObjAzimA->setEnabled(checked);
    ui->sBSetClObjAzimMin->setEnabled(ui->cBSetClObjAzimA->isEnabled()&& ui->cBSetClObjAzimA->isChecked());
    ui->sBSetClObjAzimMax->setEnabled(ui->cBSetClObjAzimA->isEnabled() && ui->cBSetClObjAzimA->isChecked());
}

void DialogConfig::showHideSetClObjAzimAct(bool checked){
    ui->sBSetClObjAzimMin->setEnabled(checked);
    ui->sBSetClObjAzimMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjRcsVal(bool checked){
    ui->cBSetClObjRcsA->setEnabled(checked);
    ui->sBSetClObjRcsMin->setEnabled(ui->cBSetClObjRcsA->isEnabled()&& ui->cBSetClObjRcsA->isChecked());
    ui->sBSetClObjRcsMax->setEnabled(ui->cBSetClObjRcsA->isEnabled() && ui->cBSetClObjRcsA->isChecked());
}

void DialogConfig::showHideSetClObjRcsAct(bool checked){
    ui->sBSetClObjRcsMin->setEnabled(checked);
    ui->sBSetClObjRcsMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVRelOncVal(bool checked){
    ui->cBSetClObjVOncA->setEnabled(checked);
    ui->sBSetClObjVOncMin->setEnabled(ui->cBSetClObjVOncA->isEnabled()&& ui->cBSetClObjVOncA->isChecked());
    ui->sBSetClObjVOncMax->setEnabled(ui->cBSetClObjVOncA->isEnabled() && ui->cBSetClObjVOncA->isChecked());
}

void DialogConfig::showHideSetClObjVRelOncAct(bool checked){
    ui->sBSetClObjVOncMin->setEnabled(checked);
    ui->sBSetClObjVOncMax->setEnabled(checked);
}

void DialogConfig::showHideSetClObjVRelDepVal(bool checked){
    ui->cBSetClObjVDepA->setEnabled(checked);
    ui->sBSetClObjVDepMin->setEnabled(ui->cBSetClObjVDepA->isEnabled()&& ui->cBSetClObjVDepA->isChecked());
    ui->sBSetClObjVDepMax->setEnabled(ui->cBSetClObjVDepA->isEnabled() && ui->cBSetClObjVDepA->isChecked());
}

void DialogConfig::showHideSetClObjVRelDepAct(bool checked){
    ui->sBSetClObjVDepMin->setEnabled(checked);
    ui->sBSetClObjVDepMax->setEnabled(checked);
}
