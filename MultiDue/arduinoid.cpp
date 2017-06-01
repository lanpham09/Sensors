

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QWidget>
#include <string>
//#include <iostream>

//***Arduino Vendor and Product ID Setup********************************************************
//Finds the vendor and product ID for the arduino
//Vendor ID: 10755, Product ID: 61
//Vendor ID stay the same for Arduino and Product ID will change with product type (Due or Uno)
//Single usage once info is found, commented out

qDebug() << "Number of available ports: "  << QSerialPortInfo::availablePorts().length();
foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
    qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
    if(serialPortInfo.hasVendorIdentifier()){
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
    }
    qDebug() << "Has product ID: " << serialPortInfo.hasProductIdentifier();
    if(serialPortInfo.hasProductIdentifier()){
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
    }
}
//***End of Arduino Vendor and Product Setup****************************************************
