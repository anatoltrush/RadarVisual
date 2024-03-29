#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // --- version ---
    sVersion = new SoftVersion(this);

    // --- displays ---
    for (uint8_t i = 0; i < RADAR_NUM; i++){
        displays[i] = new DisplayData(this);
        displays[i]->selfCount = i;
        displays[i]->setWindowTitle("Display " + QString::number(displays[i]->selfCount));
        // bind
        displays[i]->versID = &sVersion->versID;
        displays[i]->dConfig->inUse = &inUse;
#ifdef __WIN32
        connect(displays[i]->dConfig, SIGNAL(signalCanSend(QString)),this, SLOT(sendCanFrame(QString)));
#endif
    }

    // --- threads ---
    isAppStopped = false;
    thrCanRcv   = std::thread(&MainWindow::canRcv, this);
    thrZmqRcv   = std::thread(&MainWindow::zmqRcv, this);
    thrPlayFile = std::thread(&MainWindow::playCanFile, this);
    thrLog      = std::thread(&TechLogger::dropData, &tLogger, &condVar);

    // --- logger ---
    tLogger.strDirPath = QDir::currentPath();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimerTick);
    timer->start(200);

    // --- window ---
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowTitle(this->windowTitle() +
                         " v" + QString::number(VERSION_MAJOR) + "." + QString::number(VERSION_MINOR));
    ui->pBStart->setShortcut(Qt::Key_Return);

    // --- connections ---
    connect(ui->pBSoftVersID, SIGNAL(clicked()), sVersion, SLOT(slotUpdVersion()));
    connect(ui->pBSoftVersID, SIGNAL(clicked()), sVersion, SLOT(show()));

    connect(ui->cBMirror, SIGNAL(clicked(bool)), this, SLOT(slotMirroring(bool)));
    connect(ui->cBWriteLog, SIGNAL(clicked(bool)), this, SLOT(slotLogging(bool)));

    connect(ui->pBStart, SIGNAL(clicked()), this, SLOT(slotStart()));
    connect(ui->pBAddDisplay, SIGNAL(clicked()), this, SLOT(slotAddDisplay()));

    connect(ui->rBInpCAN, SIGNAL(clicked()), this, SLOT(slotInputCAN()));
    connect(ui->rBInpZMQ, SIGNAL(clicked()), this, SLOT(slotInputZMQ()));
    connect(ui->rBInpFile, SIGNAL(clicked()), this, SLOT(slotInputFile()));

    connect(ui->pBLoadFile, SIGNAL(clicked()), this, SLOT(slotLoadFile()));
    connect(ui->pBPlayFile, SIGNAL(clicked()), this, SLOT(slotPlayFile()));
    connect(ui->pBStopFile, SIGNAL(clicked()), this, SLOT(slotStopFile()));

#ifdef __WIN32
    connect(ui->cBCanPlugin, SIGNAL(currentTextChanged(QString)), this, SLOT(pluginChanged(QString)));
    connect(ui->cBCanName, SIGNAL(currentTextChanged(QString)), this, SLOT(interfaceChanged(QString)));

    ui->lEInpCAN->hide();
    QLabel* lPlug = new QLabel(this);
    lPlug->setText("plugin:");
    QLabel* lInName = new QLabel(this);
    lInName->setText("interf name:");
    ui->hLInputs->insertWidget(1, lPlug);
    ui->hLInputs->insertWidget(3, lInName);
    ui->cBCanPlugin->addItems(QCanBus::instance()->plugins());
    this->resize(600, this->height());
#else
    ui->cBCanPlugin->hide();
    ui->cBCanName->hide();
    ui->gBCanExtInfo->hide();
    this->resize(580, this->height());
#endif

    // --- placing ---
    QScreen* screen = QApplication::primaryScreen();
    int wScrn = screen->geometry().width();
    int hScrn = screen->geometry().height();

    int wFir = this->width();
    int wSec = displays[0]->width();
    int hSec = displays[0]->height();

    int fullW = wFir + wSec;
    int sideGap = (wScrn - fullW) / 2;
    int upGap = (hScrn - hSec) / 2;

    this->move(sideGap, upGap);

    // --- click ---
    ui->pBAddDisplay->click();
}

MainWindow::~MainWindow(){
#ifdef __WIN32
    if(canDevice) canDevice->disconnectDevice();
#endif

    isAppStopped = true;
    if (thrCanRcv.joinable()) thrCanRcv.join();
    if (thrPlayFile.joinable()) thrPlayFile.join();
    if (thrZmqRcv.joinable()) thrZmqRcv.join();
    tLogger.isRunning = false;
    if(thrLog.joinable()) thrLog.join();

    for (uint8_t i = 0; i < RADAR_NUM; i++)
        delete displays[i];

    zmqSubscriber.stop();

    delete ui;
}

void MainWindow::slotStart(){
    // --- --- --- from CAN --- --- ---
    if(ui->rBInpCAN->isChecked()){
#ifdef __WIN32
        bool isCanOpened = connectDevice();
        if(isCanOpened){
            ui->pBStart->setStyleSheet("background-color: green");
            ui->rBInpZMQ->setEnabled(false);
            ui->rBInpFile->setEnabled(false);
            ui->pBLoadFile->setEnabled(false);
            // --- status bar ---
            for (uint8_t i = 0; i < RADAR_NUM; i++){
                displays[i]->statusBar()->showMessage("Source: CAN (" + canSets.pluginName+"|"+canSets.deviceInterfaceName + ")");
                displays[i]->dConfig->deviceName = canSets.deviceInterfaceName.toStdString();
            }
            // --- in use ---
            inUse = InUse::can;
        }
        else{
            ui->pBStart->setStyleSheet("background-color: red");
        }
#else
        if(ui->lEInpCAN->text().isEmpty()){
            QMessageBox::information(this, "Input from CAN", "Empty CAN name");
            return;
        }
        // --- --- ---
        bool isCanOpened = openCan(ui->lEInpCAN->text().toStdString());
        if(isCanOpened){
            ui->pBStart->setStyleSheet("background-color: green");
            ui->rBInpZMQ->setEnabled(false);
            ui->rBInpFile->setEnabled(false);
            ui->pBLoadFile->setEnabled(false);
            // --- status bar ---
            for (uint8_t i = 0; i < RADAR_NUM; i++){
                displays[i]->statusBar()->showMessage("Source: physical CAN (" + QString::fromStdString(deviceName) + ")");
                displays[i]->dConfig->deviceName = deviceName;
            }
            // --- in use ---
            inUse = InUse::can;
        }
        else{
            ui->pBStart->setStyleSheet("background-color: red");
        }
#endif
    }

    // --- --- --- from ZMQ --- --- ---
    if(ui->rBInpZMQ->isChecked()){
        if(ui->lEInpZmq->text().isEmpty()){
            QMessageBox::information(this, "Input from ZMQ", "Empty input data");
            return;
        }
        // --- --- ---
        zmqSubscriber.stop();
        zmqAddrRcv = ui->lEInpZmq->text();

        zmqSubscriber.set_block_mode(true);
        zmqSubscriber.set_queue_size(0);
        zmqSubscriber.set_queue_size(0);
        zmqSubscriber.configure(zmqAddrRcv.toStdString());

        std::string what;
        isSubscrStarted = zmqSubscriber.start(what);
        if(!isSubscrStarted){
            ui->pBStart->setStyleSheet("background-color: red");
            QMessageBox::information(this, "Input from ZMQ", "Can't start ZMQ:\n" + QString::fromStdString(what));
            return;
        }

        ui->pBStart->setStyleSheet("background-color: green");
        ui->rBInpCAN->setEnabled(false);
        ui->rBInpFile->setEnabled(false);
        ui->pBLoadFile->setEnabled(false);
        // --- status bar ---
        for (uint8_t i = 0; i < RADAR_NUM; i++)
            displays[i]->statusBar()->showMessage("Source: ZMQ (" + zmqAddrRcv + "; can" + QString::number(msgId._msg_src) + ")");
        // --- in use ---
        inUse = InUse::zmq;
    }

    // --- --- --- from FILE --- --- ---
    if(ui->rBInpFile->isChecked()){
        if(ui->lEInpFile->text().isEmpty()){
            QMessageBox::information(this, "Input from log file", "Empty input data");
            return;
        }
    }
}

void MainWindow::slotInputCAN(){
    ui->pBStart->setEnabled(true);
}

#ifdef __WIN32
int MainWindow::sendCanFrame(const QString &canStr){
    if(!canDevice) return -1;

    // ---
    QStringList strList1 = canStr.split(u' ', Qt::SkipEmptyParts);
    if(strList1.size() < 3) return -2;
    QStringList strList2 = strList1[2].split(u'#', Qt::SkipEmptyParts);
    if(strList2.size() < 2) return -3;

    // --- frame ---
    uint32_t frameId = strList2[0].toUInt(nullptr, 16);
    QByteArray payload = QByteArray::fromHex(strList2[1].remove(QChar('.')).toLatin1());

    QCanBusFrame frame(frameId, payload);

    // ---
    bool isWritten = canDevice->writeFrame(frame);

    return !isWritten;
}

void MainWindow::pluginChanged(const QString &plugin){
    ui->cBCanName->clear();
    interfaces = QCanBus::instance()->availableDevices(plugin);
    for (const QCanBusDeviceInfo &info : qAsConst(interfaces))
        ui->cBCanName->addItem(info.name());
}

void MainWindow::interfaceChanged(const QString &interf){
    ui->isVirtual->setChecked(false);
    ui->isFlexibleDataRateCapable->setChecked(false);

    for (const QCanBusDeviceInfo &info : qAsConst(interfaces)) {
        if (info.name() == interf) {
            ui->descriptionLabel->setText("Descr: " + info.description());
            QString serialNumber = info.serialNumber();
            if (serialNumber.isEmpty())
                serialNumber = tr("n/a");
            ui->serialNumberLabel->setText(tr("Serial: %1").arg(serialNumber));
            QString alias = info.alias();
            if (alias.isEmpty())
                alias = tr("n/a");
            ui->aliasLabel->setText(tr("Alias: %1").arg(alias));
            ui->channelLabel->setText(tr("Channel: %1").arg(info.channel()));
            ui->isVirtual->setChecked(info.isVirtual());
            ui->isFlexibleDataRateCapable->setChecked(info.hasFlexibleDataRate());
            break;
        }
    }
}
#endif

void MainWindow::slotInputZMQ(){
    ui->pBStart->setEnabled(true);
}

void MainWindow::slotInputFile(){
    ui->pBStart->setEnabled(false);

    pathFileCanLog = QFileDialog::getOpenFileName(this, tr("Open CAN log"), "", tr("Log files (*.log)"));
    if(!pathFileCanLog.isEmpty())
        ui->lEInpFile->setText(pathFileCanLog);
}

#ifdef __WIN32
bool MainWindow::connectDevice(){
    canSets.pluginName = ui->cBCanPlugin->currentText();
    canSets.deviceInterfaceName = ui->cBCanName->currentText();

    QString errorString;
    canDevice.reset(QCanBus::instance()->createDevice(canSets.pluginName, canSets.deviceInterfaceName, &errorString));
    if (!canDevice){
        QMessageBox::information(this, "Input from CAN", "Can't start CAN:\n" +
                                 tr("Error creating device '%1', reason: '%2'").arg(canSets.pluginName).arg(errorString));
        return false;
    }

    connect(canDevice.get(), &QCanBusDevice::errorOccurred, this, &MainWindow::procErrors);
    connect(canDevice.get(), &QCanBusDevice::framesReceived, this, &MainWindow::procReceivedFrames);

    if (!canDevice->connectDevice()) {
        QMessageBox::information(this, "CAN connection...", tr("Connection error: %1").arg(canDevice->errorString()));
        canDevice.reset();
        return false;
    }
    else {
        const QVariant bitRate = canDevice->configurationParameter(QCanBusDevice::BitRateKey);
        if (bitRate.isValid()) {
            const bool isCanFd = canDevice->configurationParameter(QCanBusDevice::CanFdKey).toBool();
            const QVariant dataBitRate = canDevice->configurationParameter(QCanBusDevice::DataBitRateKey);
            if(isCanFd && dataBitRate.isValid()){
                ui->lStatus->setText(tr("Status: plugin: %1,\nconn. to %2 at %3 / %4 kBit/s")
                                  .arg(canSets.pluginName).arg(canSets.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000).arg(dataBitRate.toInt() / 1000));
            }
            else{
                ui->lStatus->setText(tr("Status: plugin: %1,\nconn. to %2 at %3 kBit/s")
                                  .arg(canSets.pluginName).arg(canSets.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000));
            }
        }
        else {
            ui->lStatus->setText(tr("Status: plugin: %1,\nconn. to %2")
                              .arg(canSets.pluginName).arg(canSets.deviceInterfaceName));
        }
        statusRadMess = "Connected";
        return true;
    }
}

void MainWindow::procErrors(QCanBusDevice::CanBusError error) const{
    switch (error){
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        ui->statBar->showMessage(canDevice->errorString());
        break;
    default:
        break;
    }
}

void MainWindow::procReceivedFrames(){
    if (!canDevice) return;

    while (canDevice->framesAvailable()) {
        numFramesReceived++;
        const QCanBusFrame frame = canDevice->readFrame();

        QString data;
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            data = canDevice->interpretErrorFrame(frame);
        else
            data = QLatin1String(frame.payload().toHex(' ').toUpper());
        QString tmSec = QString::number(frame.timeStamp().seconds());
        QString tmMcs = QString::number(frame.timeStamp().microSeconds());
        while (tmMcs.length() < 6) tmMcs.prepend("0");
        QString time = tmSec + tmMcs;
        const QString id = QString::number(frame.frameId(), 16);
        // --- --- ---
        CanLine rcvLine;
        rcvLine.canNum = canSets.deviceInterfaceName;
        rcvLine.messId = id;
        rcvLine.messData = data;
        rcvLine.messData.remove(QChar(' '));
        rcvLine.timeStamp = time.toDouble();
        sendToDisplay(rcvLine);

        // --- status bar ---
        if(numFramesReceived % MSG_NUM_CAN_STEP == 0){
            QString statLocalMess = statusRadMess + " | " + QString::number(sizeof(frame)) +
                            " bytes received. Msg num: " + QString::number(numFramesReceived);
            ui->statBar->showMessage(statLocalMess);
        }
    }
}

#else
bool MainWindow::openCan(const std::string &device){
    isCanOpened = false;
    handle = socket(PF_CAN, SOCK_RAW, CAN_RAW);    
    if(handle != -1){
        can_err_mask_t err_mask = CAN_ERR_MASK;
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

        int loopback = 1; /* 0 = disabled, 1 = enabled (default) */
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

        int recv_own_msgs = 1; /* 0 = disabled (default), 1 = enabled */
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));

        sockAddr.can_family = AF_CAN;

        if(device.empty()) sockAddr.can_ifindex = 0;
        else{
            strcpy(ifr.ifr_name, device.c_str());
            if(ioctl(handle, SIOCGIFINDEX, &ifr) == 0){
                sockAddr.can_ifindex = ifr.ifr_ifindex;
            }
            else{
                statusRadMess = "Failed to retrieve the interface index. Error: ";
                statusRadMess.append(strerror(errno));
                ui->statBar->showMessage(statusRadMess);
                return false;
            }
        }

        int enable_canfd = 0;
        statusRadMess = "CAN interf. ->" + QString::fromStdString(device) + "<-";
        if(setsockopt(handle, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) == 0)
            statusRadMess.append(" is set to CAN FD capable");
        else
            statusRadMess.append(" is not CAN FD capable");
        ui->statBar->showMessage(statusRadMess);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms
        setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        if(bind(handle, reinterpret_cast<struct sockaddr *>(&sockAddr), sizeof(sockAddr)) == 0){
            isCanOpened = true;
            deviceName = device;
        }
        else{
            statusRadMess = "Failed to bind socket. Error: ";
            statusRadMess.append(strerror(errno));
            ui->statBar->showMessage(statusRadMess);
        }
    }
    else{
        statusRadMess = "Can't open socket ";
        statusRadMess.append(strerror(errno));
        ui->statBar->showMessage(statusRadMess);
    }
    return isCanOpened;
}
#endif

void MainWindow::zmqRcv(){
    while (!isAppStopped) {
        std::this_thread::sleep_for(std::chrono::microseconds(delayZmqUs));
        if(!isSubscrStarted) continue;
        delayZmqUs = 3;

        zmq::message_t message;
        if(zmqSubscriber.receive(&message)){
            canfd_frame pframe;
            Converter::getCanFdFromZmq(message, pframe, msgId);
            CanLine canLine = Converter::getCanLineFromCanFd(std::to_string(msgId._msg_src), pframe, true);
            sendToDisplay(canLine);
            // --- status bar ---
            if(msgId._msg_num % MSG_NUM_ZMQ_STEP == 0){
                statusRadMess = "ZMQ connected (" + zmqAddrRcv + "). Msg num: " + QString::number(msgId._msg_num);
                ui->statBar->showMessage(statusRadMess);
            }
        }
    }
}

void MainWindow::canRcv(){
    QString statLocalMess;
    canfd_frame pframe;
    int nbytes = 0;
    while(!isAppStopped){
        std::this_thread::sleep_for(std::chrono::microseconds(delayCanUs));
        if(!isCanOpened) continue;
        delayCanUs = 5;

        nbytes = read(handle, &pframe, sizeof(pframe));
        if(nbytes <= 0){
            statLocalMess = statusRadMess + " | Failed to receive CAN data";
        }
        else{
            // NOTE: Receive real can data
            numFramesReceived++;
            CanLine canLine = Converter::getCanLineFromCanFd(deviceName, pframe, false);
            if (canLine.messId.size() != 3) continue;
            sendToDisplay(canLine);
        }
        // --- status bar ---
        if(numFramesReceived % MSG_NUM_CAN_STEP == 0){
            statLocalMess = statusRadMess + " | " + QString::number(nbytes) + " bytes received. Msg num: " +
                    QString::number(numFramesReceived);
            ui->statBar->showMessage(statLocalMess);
        }
    }
}

void MainWindow::fillCanLines(QFile &file, int linesAmount){
    canLines.clear();
    ui->progressBar->setValue(0);
    int percent = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList strList1 = line.split(u' ', Qt::SkipEmptyParts);
        if(strList1.size() < 3) continue;;

        CanLine canLine;
        QString tmStmp = strList1[0];
        tmStmp.remove(0, 1);
        tmStmp.chop(1);
        tmStmp.remove(QChar('.'));
        canLine.timeStamp = tmStmp.toDouble();
        canLine.canNum = strList1[1];

        QStringList strList2 = strList1[2].split(u'#', Qt::SkipEmptyParts);
        if(strList2.size() < 2) continue;;
        canLine.messId = strList2[0];
        canLine.messData = strList2[1];
        canLines.push_back(canLine);

        percent = (canLines.size() / (float)linesAmount) * 100;
        ui->progressBar->setValue(percent);
    }
    if(percent >= 100) ui->statBar->showMessage("File loaded: " + pathFileCanLog);
}

void MainWindow::playCanFile(){
    while(!isAppStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(isPlay){
            size_t currInd = 0;
            if(canLines.empty()) continue;
            double diffTime = GET_CUR_TIME_MICRO - canLines.front().timeStamp;
            while (isPlay) {
                std::this_thread::sleep_for(std::chrono::microseconds(3));
                if(isAppStopped) break;
                if(GET_CUR_TIME_MICRO >= (canLines[currInd].timeStamp + diffTime)){
                    /*std::cout << currInd << " | "<< GET_CUR_TIME_MICRO << " | " << canLines[currInd].messId.toStdString()
                              << " | " << canLines[currInd].messData.toStdString() << std::endl;*/
                    // --- send to display ---
                    if (canLines[currInd].messId.size() != 3){
                        currInd++;
                        continue;
                    }
                    sendToDisplay(canLines[currInd]);
                    // --- stop ---
                    if(currInd >= (canLines.size() - 1)){
                        isPlay = false;
                        ui->pBStopFile->setEnabled(false);
                        ui->pBPlayFile->setEnabled(true);
                        ui->pBLoadFile->setEnabled(true);
                    }
                    else{
                        currInd++;
                        int percent = ((currInd + 1) / (float)canLines.size()) * 100;
                        ui->pBPlayFile->setText("play |> " + QString::number(percent) + "%");
                    }
                }
            }
        }
    }
}

void MainWindow::slotAddDisplay(){
    for (uint8_t i = 0; i < RADAR_NUM; i++){
        if(displays[i]->isHidden()){
            displays[i]->move(this->geometry().topRight().x(), this->pos().y());
            displays[i]->show();
            return;
        }
    }
    QMessageBox::information(this, "Info", "All displays shown");
}

void MainWindow::slotTimerTick(){
    if(ui->cBWriteLog->isChecked()){
        uint8_t len = 10;
        QString strIndic = QString(len, '-');
        ui->lLogIndicator->setStyleSheet("background-color: green");
        strIndic.replace(indicCount, 1, ">");
        indicCount++;
        if(indicCount >= len - 1) indicCount = 1;
        strIndic += "[.log]";
        ui->lLogIndicator->setText(strIndic);
    }
    else{
        ui->lLogIndicator->setStyleSheet("background-color: red");
        ui->lLogIndicator->setText("--- no logging ---");
    }
}

void MainWindow::slotLoadFile(){
    if(ui->lEInpFile->text().isEmpty()){
        QMessageBox::information(this, "Input from log file","Empty input data");
        return;
    }
    // --- --- ---

    isFileLoaded = false;
    QFile file(pathFileCanLog);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","File not found");
        ui->progressBar->setValue(0);
        // --- --- ---
        ui->pBPlayFile->setEnabled(false);
        return;
    }
    else{
        std::ifstream fileStream(pathFileCanLog.toLocal8Bit()); // + cyrillic
        int stringCount = std::distance(std::istream_iterator<std::string>(fileStream),
                                        std::istream_iterator<std::string>()) / 3; // 3 words in line
        fillCanLines(file, stringCount); // ...loading...
        ui->pBLoadFile->setStyleSheet("background-color: green");
        isFileLoaded = true;
        // --- --- ---
        ui->pBPlayFile->setEnabled(true);
        ui->pBStopFile->setEnabled(false);
        // ---
        ui->rBInpCAN->setEnabled(false);
        ui->rBInpZMQ->setEnabled(false);
        // --- in use ---
        inUse = InUse::file;
    }
}

void MainWindow::slotPlayFile(){
    ui->pBLoadFile->setEnabled(false);
    ui->pBPlayFile->setEnabled(false);
    ui->pBStopFile->setEnabled(true);
    isPlay = true;
    if(!canLines.empty())
        for (uint8_t i = 0; i < RADAR_NUM; i++)
            displays[i]->statusBar()->showMessage("Source: log file (" + canLines.front().canNum + ")");
}

void MainWindow::slotStopFile(){
    ui->pBLoadFile->setEnabled(true);
    ui->pBStopFile->setEnabled(false);
    ui->pBPlayFile->setEnabled(true);
    isPlay = false;
}

void MainWindow::slotMirroring(bool checked){
    for (uint8_t i = 0; i < RADAR_NUM; i++)
        displays[i]->isMirrored = checked;
}

void MainWindow::slotLogging(bool checked){
    if(checked){
        tLogger.updFileName("/RADAR_VISUAL_");
        ui->lELogPath->setText("Path: " + tLogger.getFullName());
    }
}

void MainWindow::sendToDisplay(const CanLine &canLine){
    if(canLine.messId.length() < 2 || canLine.messData.isEmpty()) return;
    // ---
    uint8_t messIdInd = canLine.messId[1].digitValue();
    for (uint8_t i = 0; i < RADAR_NUM; i++)
        if(!displays[i]->isHidden())
            if(displays[i]->dConfig->configRadar.id == messIdInd)
                displays[i]->receiveCanLine(canLine); // NOTE: Send line to display

    // --- LOGGING ---
    if(ui->cBWriteLog->isChecked()){
        QString strLine = canLine.toQString();
        std::unique_lock<std::mutex> lockRadRaw(mutLoc);
        tLogger.data.push_back(strLine.toStdString());
    }
}
