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
    void arduino_ID(const int);
    ~Dialog();



private slots:

    void readSerial();
    void readSerial1();
    void readSerial2();
    void readSerial3();
    void readSerial4();
    void readSerial5();

    void updateLCD(const int ,const QStringList );

private:
    //each arduino needs its own Serial Port and variables to hold data from it

    Ui::Dialog *ui;

    QSerialPort *arduino;
    QSerialPort *arduino1;
    QSerialPort *arduino2;
    QSerialPort *arduino3;
    QSerialPort *arduino4;
    QSerialPort *arduino5;


    QSerialPort *temp_arduino;

    //defining the arduino due
    static const quint16 arduino_due_vendor_id = 10755;  //different arduino vendors thus two different IDs
    static const quint16 arduino_due_vendor_id_2 = 9025;
    static const quint16 arduino_due_product_id = 61;    //all Due have same product ID
    QString arduino_port_name[6];
    bool arduino_is_available[6];


    QByteArray serialData;
    QByteArray serialData1;
    QByteArray serialData2;
    QByteArray serialData3;
    QByteArray serialData4;
    QByteArray serialData5;

    QString serialBuffer = "";
    QString serialBuffer1 = "";
    QString serialBuffer2 = "";
    QString serialBuffer3 = "";
    QString serialBuffer4 = "";
    QString serialBuffer5 = "";

};

#endif // DIALOG_H
