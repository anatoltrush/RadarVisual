#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <QDialog>

namespace Ui {
class DisplayData;
}

class DisplayData : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

private:
    Ui::DisplayData *ui;
};

#endif // DISPLAYDATA_H
