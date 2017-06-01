#include "arduino.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

Arduino::Arduino()
{
    arduino_is_available[0] = false;
    arduino_port_name[0] = "";


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

    //Going through each port and assiging it to the arduino
    //This currently assumes there is only one arduino, need to assign for multiple arduino
    int number_of_arduinos = -1;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {

            number_of_arduinos = number_of_arduinos + 1;

            if ( (serialPortInfo.vendorIdentifier() == arduino_due_vendor_id) ||
                        (serialPortInfo.vendorIdentifier() == arduino_due_vendor_id_2)) {

                if (serialPortInfo.productIdentifier() == arduino_due_product_id ) {
                    arduino_port_name[number_of_arduinos] = serialPortInfo.portName();
                    arduino_is_available[number_of_arduinos] = true;
                }
            }
        }
    }//end of port assignment

    qDebug() << number_of_arduinos;
    qDebug() << arduino_port_name[0];

    arduino = new QSerialPort;

    //***Arduino Configuration for each "number_of_arduinos"
    //for (int i = 0 ; i <= number_of_arduinos; i++ ){
        if(arduino_is_available[0]) {
            //if open need to open and configure the serialport
            //this is the recommended initalization sequence for arduino configuration
            arduino->setPortName(arduino_port_name[0]);
            arduino->open(QSerialPort::ReadOnly);         //Open the port and set to read data from arduino only
            arduino->setBaudRate(QSerialPort::Baud9600);
            arduino->setDataBits(QSerialPort::Data8);     //Size of a bite, most modern applications are 8 bit
            arduino->setParity(QSerialPort::NoParity);
            arduino->setStopBits(QSerialPort::OneStop);
            arduino->setFlowControl(QSerialPort::NoFlowControl);
            QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));   //function to read data

        }
        else {
            //if no port is available then give warning
            QMessageBox::warning(this, "Ports Have An Error", "Couldn't find the Due!!");
        }
    //}

}//End of Arduino() Constructor


void Arduino::readSerial() {

    qDebug() << "Serial is Reading!";

}


Arduino::~Arduino(){

    if (arduino->isOpen()) {
        arduino->close();
    }

}//End of ~Arduino() Deconstructor
