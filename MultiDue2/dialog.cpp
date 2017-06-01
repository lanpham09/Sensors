#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //********************************
    //  Arduino Serial Communication
    //********************************

    //*** GLOBAL VARIABLE DEFINITION************************************************

    arduino_is_available[0] = false;   //initialize port availablity to false
    arduino_port_name[0] == "";        //initalize port name to nothing
    serialBuffer = "";
    serialBuffer2 = "";

    //Declaring Arduinos
    arduino = new QSerialPort;
    arduino2 = new QSerialPort;


    //*** END OF GLOBAL VARIABLE DEFINITION*****************************************

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

    //*** Port Assignment for Arduinos**********************************************************
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
    }//*** End of Port Assignment***************************************************************


    //***Arduino Port Configuration for each "number_of_arduinos", Indices start at 0
    if(arduino_is_available[0]) {


        //if open need to open and configure the serialport
        //this is the recommended initalization sequence for arduino configuration
        arduino->setPortName(arduino_port_name[0]);
        arduino->open(QSerialPort::ReadWrite);         //Open the port and set to read data from arduino only
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);     //Size of a bite, most modern applications are 8 bit
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));   //function to read data

    }
    else {
        //if no port is available then give warning

        QMessageBox::warning(this, "Port 0 Error", "Couldn't find the Arduino for Port 0!");
    }

    if(arduino_is_available[1]) {

        //if open need to open and configure the serialport
        //this is the recommended initalization sequence for arduino configuration
        arduino2->setPortName(arduino_port_name[1]);
        arduino2->open(QSerialPort::ReadWrite);         //Open the port and set to read data from arduino only
        arduino2->setBaudRate(QSerialPort::Baud9600);
        arduino2->setDataBits(QSerialPort::Data8);     //Size of a bite, most modern applications are 8 bit
        arduino2->setParity(QSerialPort::NoParity);
        arduino2->setStopBits(QSerialPort::OneStop);
        arduino2->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino2, SIGNAL(readyRead()), this, SLOT(readSerial2()));   //function to read data

    }
    else {
        //if no port is available then give warning

        QMessageBox::warning(this, "Port 0 Error", "Couldn't find the Arduino for Port 0!");
    }
    //*** End of Arduino Port Configuration

}//*** End of Dialog Constructor (main function)


//*** Read and Send to LCD Screen***************************************
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
        qDebug() << "From Arduino 0";
        qDebug() << bufferSplit ;
        Dialog::updateLCD(0,0, bufferSplit[0]);
        Dialog::updateLCD(0,1, bufferSplit[1]);
        Dialog::updateLCD(0,2, bufferSplit[2]);
        Dialog::updateLCD(0,3, bufferSplit[3]);
        serialBuffer = "" ;         //resets serial buffer so its not infinitely long
        }
    
}//*** End of Read and Send to LCD Screen

void Dialog::readSerial2()
{
    serialData2 = arduino2->readAll();
    serialBuffer2 += QString::fromStdString(serialData2.toStdString());
    QStringList bufferSplit2 = serialBuffer2.split(",");
    if (bufferSplit2.length() < 5 ){
        serialData2 = arduino2->readAll();
        serialBuffer2 += QString::fromStdString(serialData2.toStdString());
    }
    else {
        qDebug() << "From Arduino 1: ";
        qDebug() << bufferSplit2 ;
        Dialog::updateLCD(1,0, bufferSplit2[0]);
        Dialog::updateLCD(1,1, bufferSplit2[1]);
        Dialog::updateLCD(1,2, bufferSplit2[2]);
        Dialog::updateLCD(1,3, bufferSplit2[3]);
        serialBuffer2 = "" ;         //resets serial buffer so its not infinitely long
    }
    
}//*** End of Read and Send to LCD Screen


//Function to update LCD
void Dialog::updateLCD(const int arduino_number,const int sensor_number,const QString sensor_reading) {
    if (arduino_number == 0) {
        if (sensor_number == 0) {
            ui->Post_lcdNumber->display(sensor_reading);
        }
        else if (sensor_number == 1) {
            ui->Ver_lcdNumber->display(sensor_reading);
        }
        else if (sensor_number == 2) {
            ui->Hor_lcdNumber->display(sensor_reading);
        }
        else{
            ui->Omni_lcdNumber->display(sensor_reading);
        }
    }
    else if (arduino_number == 1){
        if (sensor_number == 0) {
            ui->Post_lcdNumber_2->display(sensor_reading);
        }
        else if (sensor_number == 1) {
            ui->Ver_lcdNumber_2->display(sensor_reading);
        }
        else if (sensor_number == 2) {
            ui->Hor_lcdNumber_2->display(sensor_reading);
        }
        else{
            ui->Omni_lcdNumber_2->display(sensor_reading);
        }
    }
}

//***Deconstructor*******************************************************
Dialog::~Dialog()
{
    //if the arduino port is open we also have to close it
    if (arduino->isOpen()) {
        arduino->close();
    }
    if (arduino2->isOpen()){
        arduino2->close();
    }
    delete ui;
}//***End of Deconstructor***********************************************
