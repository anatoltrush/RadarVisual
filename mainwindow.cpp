#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    isCanStopped = false;

#ifdef __WIN32
#else
    thrRcv = std::thread(&MainWindow::canRcv, this);
#endif

    on_cBInpRadNum_activated(ui->cBInpRadNum->currentIndex());
}

MainWindow::~MainWindow(){
    isCanStopped = true;
    if (thrRcv.joinable()) thrRcv.join();

    delete ui;
}

void MainWindow::on_pBStartApply_clicked(){
    // from CAN
    if(ui->rBInpCAN->isChecked()){
        if(ui->lEInpCAN->text().isEmpty()){
            QMessageBox::information(this, "Input from file","Empty input data");
            return;
        }
        // --- --- ---
#ifdef __WIN32
#else
        bool isCanOpened = openCan(ui->lEInpCAN->text().toStdString());
#endif
        if(isCanOpened)
            ui->pBStartApply->setStyleSheet("background-color: green");
        else
            ui->pBStartApply->setStyleSheet("background-color: red");
    }

    // from ZMQ
    if(ui->rBInpZMQ->isChecked()){
        if(ui->lEInpZMQ->text().isEmpty()){
            QMessageBox::information(this, "Input from ZMQ","Empty input data");
            return;
        }
        // --- --- ---
    }

    // from file
    if(ui->rBInpFile->isChecked()){
        if(ui->lEInpFile->text().isEmpty()){
            QMessageBox::information(this, "Input from log file","Empty input data");
            return;
        }
        // --- --- ---
        pathFileCanLog = ui->lEInpFile->text();
        QFile file(pathFileCanLog);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            ui->pBStartApply->setStyleSheet("background-color: red");
            QMessageBox::critical(this, "Error","File not found");
            return;
        }
        else{
            int stringCount = wordsCount(pathFileCanLog.toStdString()) / 3; // 3 words in line
            fillCanLines(file, stringCount); // ...loading...
            ui->pBStartApply->setStyleSheet("background-color: green");
            playCanFile();
        }
    }
}

void MainWindow::on_rBInpCAN_clicked(){

}

void MainWindow::on_rBInpZMQ_clicked(){

}

void MainWindow::on_rBInpFile_clicked(){
    pathFileCanLog = QFileDialog::getOpenFileName(this, tr("Open CAN log"), "", tr("Log files (*.log)"));
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
                statRadMess = "Failed to retrieve the interface index. Error: ";
                statRadMess.append(strerror(errno));
                ui->statBar->showMessage(statRadMess);
                return false;
            }
        }

        int enable_canfd = 0;
        statRadMess = "CAN interface ---> " + QString::fromStdString(device) + " <---";
        if(setsockopt(handle, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) == 0)
            statRadMess.append(" is set to CAN FD capable");
        else
            statRadMess.append(" is not CAN FD capable");
        ui->statBar->showMessage(statRadMess);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms
        setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        if(bind(handle, reinterpret_cast<struct sockaddr *>(&sockAddr), sizeof(sockAddr)) == 0){
            isCanOpened = true;
            deviceName = device;
        }
        else{
            statRadMess = "Failed to bind socket. Error: ";
            statRadMess.append(strerror(errno));
            ui->statBar->showMessage(statRadMess);
        }
    }
    else{
        statRadMess = "Can't open socket ";
        statRadMess.append(strerror(errno));
        ui->statBar->showMessage(statRadMess);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::string strSource = &deviceName.back();
        if(!isCanOpened) continue;

        nbytes = read(handle, &pframe, sizeof(pframe)); // FIXME: change function

        if(nbytes <= 0){
            statLocalMess = statRadMess + " | Failed to receive CAN data";
        }
        else{
            msgNumCan++;
            statLocalMess = statRadMess + " | " + QString::number(nbytes) + " bytes received";
        }
        ui->statBar->showMessage(statLocalMess);
        //std::cout << deviceName << std::endl;
    }
}
#endif

int MainWindow::wordsCount(const std::string &fname){
    std::ifstream file(fname);
    if(!file) throw std::runtime_error("can`t open file: " + fname);
    return std::distance((std::istream_iterator<std::string>(file)), (std::istream_iterator<std::string>()));
}

void MainWindow::fillCanLines(QFile &file, int linesAmount){
    canLines.clear();
    ui->progressBar->setValue(0);
    int percent = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList strList1 = line.split(u' ', Qt::SkipEmptyParts);

        CanLine canLine;
        canLine.timeStamp = strList1[0];
        canLine.timeStamp.remove(0, 1);
        canLine.timeStamp.chop(1);
        canLine.canNum = strList1[1];

        QStringList strList2 = strList1[2].split(u'#', Qt::SkipEmptyParts);
        canLine.canId = strList2[0];
        canLine.canData = strList2[1];
        canLines.push_back(canLine);

        percent = (canLines.size() / (float)linesAmount) * 100;
        ui->progressBar->setValue(percent);
    }
    if(percent >= 100) ui->statBar->showMessage("File loaded: " + pathFileCanLog);
}

void MainWindow::playCanFile(){
    // TODO: play can lines
}
