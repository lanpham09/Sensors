#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <vector>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //********************************
    //  ARDUINO SERIAL COMMUNCATION
    //********************************
        //need to make this into a class

    //******************************************************************************
    //  GLOBAL VARIABLE DEFINITION
    //******************************************************************************

    arduino_is_available[0] = false;   //initialize port availablity to false
    arduino_port_name[0] == "";        //initalize port name to nothing



    //Declaring Arduinos
    arduino = new QSerialPort;
    arduino1 = new QSerialPort;
    arduino2 = new QSerialPort;
    arduino3 = new QSerialPort;
    arduino4 = new QSerialPort;
    arduino5 = new QSerialPort;

    //****************************************************************************
    //  END OF GLOBAL VARIABLE DEFINITION
    //****************************************************************************

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
    for (int i = 0; i <= number_of_arduinos; ++i) {
        if(arduino_is_available[i]) {

            temp_arduino = new QSerialPort;

            //if open need to open and configure the serialport
            //this is the recommended initalization sequence for arduino configuration
            temp_arduino->setPortName(arduino_port_name[i]);
            temp_arduino->open(QSerialPort::ReadWrite);         //Open the port and set to read data from arduino only
            temp_arduino->setBaudRate(QSerialPort::Baud9600);
            temp_arduino->setDataBits(QSerialPort::Data8);     //Size of a bite, most modern applications are 8 bit
            temp_arduino->setParity(QSerialPort::NoParity);
            temp_arduino->setStopBits(QSerialPort::OneStop);
            temp_arduino->setFlowControl(QSerialPort::NoFlowControl);

            //Assigns each port to one of six possible arduino serial ports
            arduino_ID(i);

        }
        else {
            //if no port is available then give warning

            QMessageBox::warning(this, "Port 0 Error", "Couldn't find the Arduino for Port 0!");
        }
    }
}
//***Identifies which arduino is connected and has it start sending data
void Dialog::arduino_ID(const int arduino_num){

    if (arduino_num == 0){
        arduino = temp_arduino;
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));   //function to read data
    }
    else if (arduino_num == 1){
        arduino1 = temp_arduino;
        QObject::connect(arduino1, SIGNAL(readyRead()), this, SLOT(readSerial1()));   //function to read data

    }
    else if (arduino_num == 2){
        arduino2 = temp_arduino;
        QObject::connect(arduino2, SIGNAL(readyRead()), this, SLOT(readSerial2()));   //function to read data

    }
    else if (arduino_num == 3){
        arduino3 = temp_arduino;
        QObject::connect(arduino3, SIGNAL(readyRead()), this, SLOT(readSerial3()));   //function to read data

    }
    else if (arduino_num == 4){
        arduino4 = temp_arduino;
        QObject::connect(arduino4, SIGNAL(readyRead()), this, SLOT(readSerial4()));   //function to read data

    }
    else if (arduino_num == 5){
        arduino5 = temp_arduino;
        QObject::connect(arduino5, SIGNAL(readyRead()), this, SLOT(readSerial5()));   //function to read data

    }
}



//************************************************************************************
//      Seperate ReadSerial() Functions for Each Arm Section
//************************************************************************************

//*** Read and Send to LCD Screen***************************************
//Need to buffer input to get the full message
//**need an update LCD screen class
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

    if (bufferSplit.length() < 6 ){

        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());
        
    }
    else {
        //else the values in bufferSplit is a good value and assign one of the splits
        //to an LCD

        int arduino_id = bufferSplit[0].toInt();
        qDebug() <<"Arduino " << arduino_id ;
        qDebug() << bufferSplit ;
        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer = "" ;         //resets serial buffer so its not infinitely long

    }
}//*** End of Read and Send to LCD Screen

void Dialog::readSerial1()
{
    serialData1 = arduino1->readAll();
    serialBuffer1 += QString::fromStdString(serialData1.toStdString());
    QStringList bufferSplit = serialBuffer1.split(",");
    if (bufferSplit.length() < 6){
        serialData1 = arduino1->readAll();
        serialBuffer1 += QString::fromStdString(serialData1.toStdString());
    }
    else {
        int arduino_id = bufferSplit[0].toInt();
        qDebug() << "Arduino " << arduino_id;
        qDebug() << bufferSplit ;

        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer1 = "" ;         //resets serial buffer so its not infinitely long
    }
 }
void Dialog::readSerial2()
{
    serialData2 = arduino2->readAll();
    serialBuffer2 += QString::fromStdString(serialData2.toStdString());
    QStringList bufferSplit = serialBuffer2.split(",");
    if (bufferSplit.length() < 6){
        serialData2 = arduino2->readAll();
        serialBuffer2 += QString::fromStdString(serialData2.toStdString());
    }
    else {
        int arduino_id = bufferSplit[0].toInt();
        qDebug() << "Arduino " << arduino_id;
        qDebug() << bufferSplit ;
        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer2 = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial3()
{
    serialData3 = arduino3->readAll();
    serialBuffer3 += QString::fromStdString(serialData3.toStdString());
    QStringList bufferSplit = serialBuffer3.split(",");
    if (bufferSplit.length() < 6){
        serialData3 = arduino3->readAll();
        serialBuffer3 += QString::fromStdString(serialData3.toStdString());
    }
    else {
        int arduino_id = bufferSplit[0].toInt();
        qDebug() << "Arduino " << arduino_id;
        qDebug() << bufferSplit ;
        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer3 = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial4()
{
    serialData4 = arduino4->readAll();
    serialBuffer4 += QString::fromStdString(serialData4.toStdString());
    QStringList bufferSplit = serialBuffer4.split(",");
    if (bufferSplit.length() < 6){
        serialData4 = arduino4->readAll();
        serialBuffer4 += QString::fromStdString(serialData4.toStdString());
    }
    else {
        int arduino_id = bufferSplit[0].toInt();
        qDebug() << "Arduino " << arduino_id;
        qDebug() << bufferSplit ;
        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer4 = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial5()
{
    serialData5 = arduino5->readAll();
    serialBuffer5 += QString::fromStdString(serialData5.toStdString());
    QStringList bufferSplit = serialBuffer5.split(",");
    if (bufferSplit.length() < 6){
        serialData5 = arduino5->readAll();
        serialBuffer5 += QString::fromStdString(serialData5.toStdString());
    }
    else {
        int arduino_id = bufferSplit[0].toInt();
        qDebug() << "Arduino " << arduino_id;
        qDebug() << bufferSplit ;
        Dialog::updateLCD(arduino_id, bufferSplit);
        serialBuffer5 = "" ;         //resets serial buffer so its not infinitely long
    }
}


//**********************************************************************
//  END of ReadSerial()
//**********************************************************************

//Function to update LCD
//Assumed sensor readings will be received in the same order every time
void Dialog::updateLCD(const int arduino_number,const QStringList sensor_reading) {
    if (arduino_number == 0) {
            ui->Post_lcdNumber->display(sensor_reading[1]);
            ui->Ver_lcdNumber->display(sensor_reading[2]);
            ui->Hor_lcdNumber->display(sensor_reading[3]);
            ui->Omni_lcdNumber->display(sensor_reading[4]);
    }
    else if (arduino_number == 1){
            ui->Post_lcdNumber_2->display(sensor_reading[1]);
            ui->Ver_lcdNumber_2->display(sensor_reading[2]);
            ui->Hor_lcdNumber_2->display(sensor_reading[3]);
            ui->Omni_lcdNumber_2->display(sensor_reading[4]);
    }
    else if (arduino_number == 2){
            ui->Post_lcdNumber_3->display(sensor_reading[1]);
            ui->Ver_lcdNumber_3->display(sensor_reading[2]);
            ui->Hor_lcdNumber_3->display(sensor_reading[3]);
            ui->Omni_lcdNumber_3->display(sensor_reading[4]);
    }
    else if (arduino_number == 3){
            ui->Post_lcdNumber_4->display(sensor_reading[1]);
            ui->Ver_lcdNumber_4->display(sensor_reading[2]);
            ui->Hor_lcdNumber_4->display(sensor_reading[3]);
            ui->Omni_lcdNumber_4->display(sensor_reading[4]);
    }
    else if (arduino_number == 4){
            ui->Post_lcdNumber_5->display(sensor_reading[1]);
            ui->Ver_lcdNumber_5->display(sensor_reading[2]);
            ui->Hor_lcdNumber_5->display(sensor_reading[3]);
            ui->Omni_lcdNumber_5->display(sensor_reading[4]);
    }
    else if (arduino_number == 5){
            ui->Post_lcdNumber_6->display(sensor_reading[1]);
            ui->Ver_lcdNumber_6->display(sensor_reading[2]);
            ui->Hor_lcdNumber_6->display(sensor_reading[3]);
            ui->Omni_lcdNumber_6->display(sensor_reading[4]);
    }
}

//***Deconstructor*******************************************************
Dialog::~Dialog()
{
    //if the arduino port is open we also have to close it
    if (arduino->isOpen()) {
        arduino->close();
    }
    if (arduino1->isOpen()){
        arduino1->close();
    }
    if (arduino2->isOpen()){
        arduino2->close();
    }
    if (arduino3->isOpen()){
        arduino3->close();
    }
    if (arduino4->isOpen()){
        arduino4->close();
    }
    if (arduino5->isOpen()){
        arduino5->close();
    }
    if (temp_arduino->isOpen()){
        temp_arduino->close();
    }
    delete ui;
}//***End of Deconstructor***********************************************
