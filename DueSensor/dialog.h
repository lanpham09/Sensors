#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


private slots:
    void readSerial();

    void updateLCD(const int ,const QString );
private:
    Ui::Dialog *ui;
    QSerialPort *arduino;

    //defining the arduino due
    static const quint16 arduino_due_vendor_id = 10755;  //oddly it is not the same the tutorial
    static const quint16 arduino_due_product_id = 61;

    QByteArray serialData;
    QString serialBuffer;

    QString arduino_port_name;
    bool arduino_is_available;
};

#endif // DIALOG_H
