#include "softversion.h"
#include "ui_softversion.h"

SoftVersion::SoftVersion(QWidget *parent) : QDialog(parent), ui(new Ui::SoftVersion){
    ui->setupUi(this);

    setFixedSize(300, 160);
    ui->lHyper->setOpenExternalLinks(true);
}

SoftVersion::~SoftVersion(){
    delete ui;
}

void SoftVersion::slotUpdVersion(){
    if(versID.major < 255){
        ui->lMajData->setText(QString::number(versID.major));
        ui->lMinData->setText(QString::number(versID.minor));
        ui->lPatData->setText(QString::number(versID.patch));
        if(versID.extended == 0)
            ui->lExtData->setText("Standard range version");
        else
            ui->lExtData->setText("Extended range version"); // Without any increase of the power of radiation
        if(versID.country == 0)
            ui->lCountryData->setText("International version");
        else
            ui->lCountryData->setText("South Korea, Japan"); // Power reduction is demanded
    }
}
