#ifndef SOFTVERSION_H
#define SOFTVERSION_H

#include <QDialog>

#include "entity.h"

namespace Ui {
class SoftVersion;
}

class SoftVersion : public QDialog{
    Q_OBJECT

public:
    explicit SoftVersion(QWidget *parent = nullptr);
    ~SoftVersion();

    VersionID versID;

private:
    Ui::SoftVersion *ui;

public slots:
    void updVersion();
};

#endif // SOFTVERSION_H
