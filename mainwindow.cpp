#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    isCanStopped = false;

    thrRcv = std::thread(&MainWindow::canRcv, this);
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
        bool isCanOpened = openCan(ui->lEInpCAN->text());
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

void MainWindow::on_cBInpRadNum_activated(int index){

}

bool MainWindow::openCan(const QString &device){
    bool isOpened = false;
    handle = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    std::string infoMess;
    if(handle != -1){
        can_err_mask_t err_mask = CAN_ERR_MASK;
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

        int loopback = 1; /* 0 = disabled, 1 = enabled (default) */
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

        int recv_own_msgs = 1; /* 0 = disabled (default), 1 = enabled */
        setsockopt(handle, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));

        sockAddr.can_family = AF_CAN;

        if(device.isEmpty()) sockAddr.can_ifindex = 0;
        else{
            //strncpy(ifr.ifr_name, device.toStdString().c_str(), device.length());
            strcpy(ifr.ifr_name, device.toStdString().c_str());
            if(ioctl(handle, SIOCGIFINDEX, &ifr) == 0){
                sockAddr.can_ifindex = ifr.ifr_ifindex;
            }
            else{
                infoMess = "Failed to retrieve the interface index. Error: ";
                infoMess.append(strerror(errno));
                ui->statBar->showMessage(QString::fromStdString(infoMess));
                return false;
            }
        }

        int enable_canfd = 0;
        infoMess = "CAN interface ---> " + device.toStdString() + " <---";
        if(setsockopt(handle, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd)) == 0)
            infoMess.append(" is set to CAN FD capable");
        else
            infoMess.append(" is not CAN FD capable");
        ui->statBar->showMessage(QString::fromStdString(infoMess));

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms
        setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        if(bind(handle, reinterpret_cast<struct sockaddr *>(&sockAddr), sizeof(sockAddr)) == 0){
            isOpened = true;
            deviceName = device.toStdString();
        }
        else{
            infoMess = "Failed to bind socket. Error: ";
            infoMess.append(strerror(errno));
            ui->statBar->showMessage(QString::fromStdString(infoMess));
        }
    }
    else{
        infoMess = "Can't open socket ";
        infoMess.append(strerror(errno));
        ui->statBar->showMessage(QString::fromStdString(infoMess));
    }
    return isOpened;
}

void MainWindow::canRcv(){
    canfd_frame pframe;
    size_t nbytes = 0;

    std::string strSrc = &deviceName.back();

    while(!isCanStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        //nbytes = read(handle, &pframe, sizeof(pframe));

        //std::cout << deviceName << std::endl;
    }
}
