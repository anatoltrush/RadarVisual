#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    isCanStopped = false;

    thrRcv = std::thread(&MainWindow::canRcv, this);

    on_cBInpRadNum_activated(ui->cBInpRadNum->currentIndex());
}

MainWindow::~MainWindow(){
    isCanStopped = true;
    if (thrRcv.joinable()) thrRcv.join();

    delete ui;
}

void MainWindow::on_pBStartApply_clicked(){
    if(ui->rBInpCAN->isChecked()){
        if(ui->lEInpCAN->text().isEmpty()){
            QMessageBox::information(this, "Info","Empty input data");
            return;
        }
        bool isCanOpened = openCan(ui->lEInpCAN->text().toStdString());
        if(isCanOpened)
            ui->pBStartApply->setStyleSheet("background-color: green");
        else
            ui->pBStartApply->setStyleSheet("background-color: red");
    }
    if(ui->rBInpZMQ->isChecked()){
        if(ui->lEInpZMQ->text().isEmpty()){
            QMessageBox::information(this, "Info","Empty input data");
            return;
        }
    }
    if(ui->rBInpFile->isChecked()){
        if(ui->lEInpFile->text().isEmpty()){
            QMessageBox::information(this, "Info","Empty input data");
            return;
        }
    }
}

void MainWindow::on_rBInpCAN_clicked(){

}

void MainWindow::on_rBInpZMQ_clicked(){

}

void MainWindow::on_rBInpFile_clicked(){

}

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
            //strncpy(ifr.ifr_name, device.toStdString().c_str(), device.length());
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

void MainWindow::canRcv(){
    QString statLocalMess;
    canfd_frame pframe;
    int nbytes = 0;
    while(!isCanStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::string strSource = &deviceName.back();
        if(!isCanOpened) continue;

        nbytes = read(handle, &pframe, sizeof(pframe));

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
