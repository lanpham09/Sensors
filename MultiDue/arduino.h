#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>

class Arduino
{
public:
    Arduino();
    ~Arduino();
   // void checkArduino();

private slots:

    void readSerial();

    //void updateLCD(const int ,const QString );

private:


    QSerialPort *arduino;

    static const quint16 arduino_due_vendor_id = 10755;  //different arduino vendors thus two different IDs
    static const quint16 arduino_due_vendor_id_2 = 9025;
    static const quint16 arduino_due_product_id = 61;    //all Due have same product ID
    QString arduino_port_name[6];
    bool arduino_is_available[6];


};

#endif // ARDUINO_H
