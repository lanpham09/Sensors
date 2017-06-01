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
    void readSerial2();

    void updateLCD(const int , const int ,const QString );
private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    QSerialPort *arduino2;

    //defining the arduino due
    static const quint16 arduino_due_vendor_id = 10755;  //different arduino vendors thus two different IDs
    static const quint16 arduino_due_vendor_id_2 = 9025;
    static const quint16 arduino_due_product_id = 61;    //all Due have same product ID
    QString arduino_port_name[6];
    bool arduino_is_available[6];

    QByteArray serialData;
    QString serialBuffer;
    QByteArray serialData2;
    QString serialBuffer2;

};

#endif // DIALOG_H
