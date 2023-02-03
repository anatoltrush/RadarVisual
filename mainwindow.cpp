#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    for (int i = 0; i < RADAR_NUM; i++){
        displays[i] = new DisplayData(this);
        displays[i]->selfCount = i;
        displays[i]->setWindowTitle("Display " + QString::number(displays[i]->selfCount));
    }

    isCanStopped = false;

#ifdef __WIN32
#else
    thrRcv = std::thread(&MainWindow::canRcv, this);
#endif
    thrPlayFile = std::thread(&MainWindow::playCanFile, this);

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
    on_pBAddDisplay_clicked();
    // ---
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    ui->pBStart->setShortcut(Qt::Key_Return);
}

MainWindow::~MainWindow(){
    isCanStopped = true;
    if (thrRcv.joinable()) thrRcv.join();
    if (thrPlayFile.joinable()) thrPlayFile.join();

        for (int i = 0; i < RADAR_NUM; i++)
            delete displays[i];

    delete ui;
}

void MainWindow::on_pBStart_clicked(){
    // --- --- --- from CAN --- --- ---
    if(ui->rBInpCAN->isChecked()){
        if(ui->lEInpCAN->text().isEmpty()){
            QMessageBox::information(this, "Input from file", "Empty input data");
            return;
        }
        // --- --- ---
#ifdef __WIN32
#else
        bool isCanOpened = openCan(ui->lEInpCAN->text().toStdString());
        if(isCanOpened){
            ui->pBStart->setStyleSheet("background-color: green");
            ui->rBInpZMQ->setEnabled(false);
            ui->rBInpFile->setEnabled(false);
            // --- status bar ---
            for (int i = 0; i < RADAR_NUM; i++)
                displays[i]->statusBar()->showMessage("Source: physical CAN (" + QString::fromStdString(deviceName) + ")");
        }
        else{
            ui->pBStart->setStyleSheet("background-color: red");
        }
#endif
    }

    // --- --- --- from ZMQ --- --- ---
    if(ui->rBInpZMQ->isChecked()){
        if(ui->lEInpZMQ->text().isEmpty()){
            QMessageBox::information(this, "Input from ZMQ", "Empty input data");
            return;
        }
        // --- --- ---
        addressString = ui->lEInpZMQ->text();
    }

    // --- --- --- from FILE --- --- ---
    if(ui->rBInpFile->isChecked()){
        if(ui->lEInpFile->text().isEmpty()){
            QMessageBox::information(this, "Input from log file", "Empty input data");
            return;
        }
    }
}

void MainWindow::on_rBInpCAN_clicked(){
    ui->pBStart->setEnabled(true);
}

void MainWindow::on_rBInpZMQ_clicked(){
    ui->pBStart->setEnabled(true);
}

void MainWindow::on_rBInpFile_clicked(){
    ui->pBStart->setEnabled(false);

    pathFileCanLog = QFileDialog::getOpenFileName(this, tr("Open CAN log"), "", tr("Log files (*.log)"));
    if(!pathFileCanLog.isEmpty())
        ui->lEInpFile->setText(pathFileCanLog);
}

#ifdef __WIN32
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
        statusRadMess = "CAN interface ---> " + QString::fromStdString(device) + " <---";
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

#ifdef __WIN32
#else
void MainWindow::canRcv(){
    QString statLocalMess;
    canfd_frame pframe;
    int nbytes = 0;
    while(!isCanStopped){
        std::this_thread::sleep_for(std::chrono::microseconds(5));

        std::string strSource = &deviceName.back();
        if(!isCanOpened) continue;

        nbytes = read(handle, &pframe, sizeof(pframe));

        if(nbytes <= 0){
            statLocalMess = statusRadMess + " | Failed to receive CAN data";
        }
        else{
            // NOTE: Receive real can data
            statLocalMess = statusRadMess + " | " + QString::number(nbytes) + " bytes received";
            CanLine canLine = Converter::getCanLineFromCanData(deviceName, pframe);
            if (canLine.messId.size() != 3) continue;
            sendToDisplay(canLine);
        }
        ui->statBar->showMessage(statLocalMess);
    }
}
#endif

void MainWindow::fillCanLines(QFile &file, int linesAmount){
    canLines.clear();
    ui->progressBar->setValue(0);
    int percent = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList strList1 = line.split(u' ', Qt::SkipEmptyParts);
        if(strList1.size() < 3) break;

        CanLine canLine;
        QString tmStmp = strList1[0];
        tmStmp.remove(0, 1);
        tmStmp.chop(1);
        tmStmp.remove(QChar('.'));
        canLine.timeStamp = tmStmp.toDouble();
        canLine.canNum = strList1[1];

        QStringList strList2 = strList1[2].split(u'#', Qt::SkipEmptyParts);
        if(strList2.size() < 2) break;
        canLine.messId = strList2[0];
        canLine.messData = strList2[1];
        canLines.push_back(canLine);

        percent = (canLines.size() / (float)linesAmount) * 100;
        ui->progressBar->setValue(percent);
    }
    if(percent >= 100) ui->statBar->showMessage("File loaded: " + pathFileCanLog);
}

void MainWindow::playCanFile(){
    while(!isCanStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if(isPlay){
            size_t currInd = 0;
            if(canLines.empty()) continue;
            double diffTime = GET_CUR_TIME_MICRO - canLines.front().timeStamp;
            while (isPlay) {
                std::this_thread::sleep_for(std::chrono::microseconds(3));
                if(isCanStopped) break;
                if(GET_CUR_TIME_MICRO >= (canLines[currInd].timeStamp + diffTime)){
                    /*std::cout << currInd << " | "<< GET_CUR_TIME_MICRO << " | " << canLines[currInd].messId.toStdString()
                              << " | " << canLines[currInd].messData.toStdString() << std::endl;*/
                    // --- send to display ---
                    if (canLines[currInd].messId.size() != 3) continue;
                    sendToDisplay(canLines[currInd]);
                    // --- stop ---
                    if(currInd >= (canLines.size() - 1)){
                        isPlay = false;
                        ui->pBStopFile->setEnabled(false);
                        ui->pBPlayFile->setEnabled(true);
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

void MainWindow::on_pBAddDisplay_clicked(){
    for (int i = 0; i < RADAR_NUM; i++){
        if(displays[i]->isHidden()){
            displays[i]->move(this->geometry().topRight().x(), this->pos().y());
            displays[i]->show();
            return;
        }
    }
    QMessageBox::information(this, "Info", "All displays shown");
}

void MainWindow::on_pBLoadFile_clicked(){
    if(ui->lEInpFile->text().isEmpty()){
        QMessageBox::information(this, "Input from log file","Empty input data");
        return;
    }
    // --- --- ---

    isFileLoaded = false;
    pathFileCanLog = ui->lEInpFile->text();
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
        std::ifstream fileStream(pathFileCanLog.toStdString());
        int stringCount = std::distance((std::istream_iterator<std::string>(fileStream)),
                                        (std::istream_iterator<std::string>())) / 3; // 3 words in line
        fillCanLines(file, stringCount); // ...loading...
        ui->pBLoadFile->setStyleSheet("background-color: green");
        isFileLoaded = true;
        // --- --- ---
        ui->pBPlayFile->setEnabled(true);
        ui->pBStopFile->setEnabled(false);
        // ---
        ui->rBInpCAN->setEnabled(false);
        ui->rBInpZMQ->setEnabled(false);
    }
}

void MainWindow::on_pBPlayFile_clicked(){
    ui->pBLoadFile->setEnabled(false);
    ui->pBPlayFile->setEnabled(false);
    ui->pBStopFile->setEnabled(true);
    isPlay = true;
    if(!canLines.empty())
        for (int i = 0; i < RADAR_NUM; i++)
            displays[i]->statusBar()->showMessage("Source: log file (" + canLines.front().canNum + ")");
}

void MainWindow::on_pBStopFile_clicked(){
    ui->pBLoadFile->setEnabled(true);
    ui->pBStopFile->setEnabled(false);
    ui->pBPlayFile->setEnabled(true);
    isPlay = false;
}

void MainWindow::sendToDisplay(const CanLine &canLine){
    uint8_t messIdInd = canLine.messId[1].digitValue();
    for (int i = 0; i < RADAR_NUM; i++)
        if(!displays[i]->isHidden())
            if(displays[i]->currRadInd == messIdInd) // NOTE: Send line to display
                displays[i]->receiveCanLine(canLine);
}
