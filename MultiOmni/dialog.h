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
    void arduino_connect(const int);
    ~Dialog();

    int number_of_arduinos;
   bool arduino_is_available[6];


private slots:

    void readSerial();

    void updateLCD(const int ,const QStringList );

private:
    //each arduino needs its own Serial Port and variables to hold data from it

    Ui::Dialog *ui;

    QSerialPort *arduino[6];

    //defining the arduino due
    void find_arduino_IDinfo();
    static const quint16 arduino_due_vendor_id = 10755;  //different arduino vendors thus two different IDs
    static const quint16 arduino_due_vendor_id_2 = 9025;
    static const quint16 arduino_due_product_id = 61;    //all Due have same product ID

    //Arduino Data Variables
    QString arduino_port_name[6];
    int arduino_id[6];
    QByteArray serialData[6];
    QString serialBuffer[6];
    QStringList bufferSplit[6];

    //arduino functions
    void connect_serialport(int i);
    void port_assignment();

};

#endif // DIALOG_H
