#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

#include <string>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->DueSensorlcdNumber->display("-------");


    //********************************
    //  Arduino Serial Communication
    //********************************

    arduino_is_available = false;   //initialize port availablity to false
    arduino_port_name == "";        //initalize port name to nothing
    arduino = new QSerialPort;
    serialBuffer = "";
    /*
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
    */

    //goes through each available serial port, checks to see if is an Arduino due and if true it finds the
    //port name
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if (serialPortInfo.vendorIdentifier() == arduino_due_vendor_id) {
                if (serialPortInfo.productIdentifier() == arduino_due_product_id) {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available) {
        //if open need to open and configure the serialport
        //this is the recommended initalization sequence for arduino configuration
      arduino->setPortName(arduino_port_name);
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
        QMessageBox::warning(this, "Port Error", "Couldn't find the Arduino!");
    }
}

//when the diolog box is closed it closes the UI and the arduino
Dialog::~Dialog()
{
    //if the arduino port is open we also have to close it
    if (arduino->isOpen()) {
        arduino->close();
    }
    delete ui;
}

//Need to buffer input to get the full message
void Dialog::readSerial()
{
    //Reads all data coming from arduino
    serialData = arduino->readAll();
    //takes the data and puts it into a long string one after the other
    serialBuffer += QString::fromStdString(serialData.toStdString());
    //lets you see what is the in serialBuffer
    //qDebug() << serialBuffer;

    //splits the serial buffer by "," to get individual values
    QStringList bufferSplit = serialBuffer.split(",");
    if (bufferSplit.length() < 5 ){

        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());

    }
    else {
        //else the values in bufferSplit is a good value and assign one of the splits
        //to an LCD
        qDebug() << bufferSplit ;
        Dialog::updateLCD(0, bufferSplit[0]);
        Dialog::updateLCD(1, bufferSplit[1]);
        Dialog::updateLCD(2, bufferSplit[2]);
        Dialog::updateLCD(3, bufferSplit[3]);
        serialBuffer = "" ;         //resets serial buffer so its not infinitely long
    }

}

//Function to update LCD
void Dialog::updateLCD(const int sensor_number,const QString sensor_reading) {
    if (sensor_number == 0) {

        ui->DueSensorlcdNumber->display(sensor_reading);

    }
    else if (sensor_number == 1) {
        ui->DueSensorlcdNumber_2->display(sensor_reading);
    }
    else if (sensor_number == 2) {

        ui->DueSensorlcdNumber_3->display(sensor_reading);
    }
    else{

        ui->DueSensorlcdNumber_4->display(sensor_reading);
    }
}

