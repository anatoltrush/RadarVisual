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

    }
    if(ui->rBInpZMQ->isChecked()){

    }
    if(ui->rBInpFile->isChecked()){

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

void MainWindow::canRcv(){
    canfd_frame pframe;
    size_t nbytes = 0;
    while(!isCanStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout << "MainWindow::canRcv()" << std::endl;
    }
}
