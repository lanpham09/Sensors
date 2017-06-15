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


    for (int i = 0; i < 6; ++i){
        arduino_is_available[0] = false;   //initialize port availablity to false
        arduino_port_name[0] == "";        //initalize port name to nothing
        serialBuffer[i] = "";              //initalize serialBuffer to nothing
    }

    //Declaring Arduinos, each arduino is a unique port, currently can't put into array
    arduino = new QSerialPort;
    arduino1 = new QSerialPort;
    arduino2 = new QSerialPort;
    arduino3 = new QSerialPort;
    arduino4 = new QSerialPort;
    arduino5 = new QSerialPort;

    //****************************************************************************
    //  END OF GLOBAL VARIABLE DEFINITION
    //****************************************************************************

    //Function which determins which ports are connected to a Due. A Due is idenfied
    //by comparing product and vendor ID from the function find_arduino_IDinfo()
    port_assignment();

    //For loop which goes through each available Due port in order to configure using the
    //configure_serialport(int ) function and than connects to it via the
    //arduion_conection (int ) function
    for (int i = 0; i <= number_of_arduinos; ++i) {

        configure_serialport(i);
        //Assigns each port to one of six possible arduino serial ports
        arduino_connect(i);

    }

}//**End of Arduino Constructor

void Dialog::port_assignment(){

    /*Runs though each available port on the computer (include ones not connected to Dues)
     *and what the vendor and product ID of that connection is. Using the information from
     * find_arduino_IDinfo() function the specific product ID and vendor IDs (not more than one)
     * a Due is idenfied. The specific port of that Due is stored into an array and the availablity
     * of that slot number is made true.
     */

    number_of_arduinos = -1;
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
    }

}//*** End of Port Assignment*********

void Dialog::configure_serialport(int i){
    //***Arduino Port Configuration for each "number_of_arduinos", Indices start at 0
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

    }
    else {
        //if no port is available then give warning

        QMessageBox::warning(this, "Port 0 Error", "Couldn't find the Arduino for Port 0!");
    }
}//**End of arduino configuration


void Dialog::arduino_connect(const int arduino_num){
//***Identifies which arduino is connected and has it start sending data

    if (arduino_num == 0){
        arduino = temp_arduino;
        bool check_connection = QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));   //function to read data
        qDebug() << "Check Connection = " << check_connection;
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
    serialData[0] = arduino->readAll();
    //takes the data and puts it into a long string one after the other
    serialBuffer[0] += QString::fromStdString(serialData[0].toStdString());
    //lets you see what is the in serialBuffer
    //qDebug() << serialBuffer;

    //splits the serial buffer by "," to get individual values
    bufferSplit[0] = serialBuffer[0].split(",");

    if (bufferSplit[0].length() < 6 ){

        serialData[0] = arduino->readAll();
        serialBuffer[0] += QString::fromStdString(serialData[0].toStdString());

    }
    else {
        //else the values in bufferSplit is a good value and assign one of the splits
        //to an LCD

        arduino_id[0] = bufferSplit[0][0].toInt();
        qDebug() <<"Arduino " << arduino_id[0] ;
        qDebug() << bufferSplit[0] ;
        Dialog::updateLCD(arduino_id[0], bufferSplit[0]);
        serialBuffer[0] = "" ;         //resets serial buffer so its not infinitely long

    }
}//*** End of Read and Send to LCD Screen

void Dialog::readSerial1()
{
    serialData[1] = arduino1->readAll();
    serialBuffer[1] += QString::fromStdString(serialData[1].toStdString());
    bufferSplit[1] = serialBuffer[1].split(",");
    if (bufferSplit[1].length() < 6){
        serialData[1] = arduino1->readAll();
        serialBuffer[1] += QString::fromStdString(serialData[1].toStdString());
    }
    else {
        arduino_id[1] = bufferSplit[1][0].toInt();
        qDebug() << "Arduino " << arduino_id[1];
        qDebug() << bufferSplit[1] ;

        Dialog::updateLCD(arduino_id[1], bufferSplit[1]);
        serialBuffer[1] = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial2()
{
    serialData[2] = arduino2->readAll();
    serialBuffer[2] += QString::fromStdString(serialData[2].toStdString());
    bufferSplit[2] = serialBuffer[2].split(",");
    if (bufferSplit[2].length() < 6){
        serialData[2] = arduino2->readAll();
        serialBuffer[2] += QString::fromStdString(serialData[2].toStdString());
    }
    else {
        arduino_id[2] = bufferSplit[2][0].toInt();
        //Check that arduinos are receiving correctly
        qDebug() << "Arduino " << arduino_id[2];
        qDebug() << bufferSplit[2] ;
        Dialog::updateLCD(arduino_id[2], bufferSplit[2]);
        serialBuffer[2] = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial3()
{
    serialData[3] = arduino3->readAll();
    serialBuffer[3] += QString::fromStdString(serialData[3].toStdString());
    bufferSplit[3] = serialBuffer[3].split(",");
    if (bufferSplit[3].length() < 6){
        serialData[3] = arduino3->readAll();
        serialBuffer[3] += QString::fromStdString(serialData[3].toStdString());
    }
    else {
        arduino_id[3] = bufferSplit[3][0].toInt();
        qDebug() << "Arduino " << arduino_id[3];
        qDebug() << bufferSplit[3] ;
        Dialog::updateLCD(arduino_id[3], bufferSplit[3]);
        serialBuffer[3] = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial4()
{
    serialData[4] = arduino4->readAll();
    serialBuffer[4] += QString::fromStdString(serialData[4].toStdString());
    bufferSplit[4] = serialBuffer[4].split(",");
    if (bufferSplit[4].length() < 6){
        serialData[4] = arduino4->readAll();
        serialBuffer[4] += QString::fromStdString(serialData[4].toStdString());
    }
    else {
        arduino_id[4] = bufferSplit[4][0].toInt();
        qDebug() << "Arduino " << arduino_id[4];
        qDebug() << bufferSplit[4] ;
        Dialog::updateLCD(arduino_id[4], bufferSplit[4]);
        serialBuffer[4] = "" ;         //resets serial buffer so its not infinitely long
    }
}
void Dialog::readSerial5()
{
    serialData[5] = arduino5->readAll();
    serialBuffer[5] += QString::fromStdString(serialData[5].toStdString());
    bufferSplit[5] = serialBuffer[5].split(",");
    if (bufferSplit[5].length() < 6){
        serialData[5] = arduino5->readAll();
        serialBuffer[5] += QString::fromStdString(serialData[5].toStdString());
    }
    else {
        arduino_id[5] = bufferSplit[5][0].toInt();
        qDebug() << "Arduino " << arduino_id[5];
        qDebug() << bufferSplit[5] ;
        Dialog::updateLCD(arduino_id[5], bufferSplit[5]);
        serialBuffer[5] = "" ;         //resets serial buffer so its not infinitely long
    }
}


//**********************************************************************
//  END of ReadSerial()
//**********************************************************************

//Function to update LCD
//Assumed sensor readings will be received in the same order every time
//Each linear sensor is located slightly off from each other therefor the this function will
//also account for these offset in order for everyone to have the same "zero" position
//(magnet is to the table closes to the post)
void Dialog::updateLCD(const int arduino_number,const QStringList sensor_reading) {
    if (arduino_number == 0) {
        ui->Post_lcdNumber->display(sensor_reading[1]);
        ui->Hor_lcdNumber->display(sensor_reading[2]);
        ui->Ver_lcdNumber->display(sensor_reading[3]);
        ui->Omni_lcdNumber->display(sensor_reading[4]);
    }
    else if (arduino_number == 1){
        ui->Post_lcdNumber_2->display(sensor_reading[1]);
        ui->Hor_lcdNumber_2->display(sensor_reading[2]);
        ui->Ver_lcdNumber_2->display(sensor_reading[3]);
        ui->Omni_lcdNumber_2->display(sensor_reading[4]);
    }
    else if (arduino_number == 2){
        ui->Post_lcdNumber_3->display(sensor_reading[1]);
        ui->Hor_lcdNumber_3->display(sensor_reading[2]);
        ui->Ver_lcdNumber_3->display(sensor_reading[3]);
        ui->Omni_lcdNumber_3->display(sensor_reading[4]);
    }
    else if (arduino_number == 3){
        ui->Post_lcdNumber_4->display(sensor_reading[1]);
        ui->Hor_lcdNumber_4->display(sensor_reading[2]);
        ui->Ver_lcdNumber_4->display(sensor_reading[3]);
        ui->Omni_lcdNumber_4->display(sensor_reading[4]);
    }
    else if (arduino_number == 4){
        ui->Post_lcdNumber_5->display(sensor_reading[1]);
        ui->Hor_lcdNumber_5->display(sensor_reading[2]);
        ui->Ver_lcdNumber_5->display(sensor_reading[3]);
        ui->Omni_lcdNumber_5->display(sensor_reading[4]);
    }
    else if (arduino_number == 5){
        ui->Post_lcdNumber_6->display(sensor_reading[1]);
        ui->Hor_lcdNumber_6->display(sensor_reading[2]);
        ui->Ver_lcdNumber_6->display(sensor_reading[3]);
        ui->Omni_lcdNumber_6->display(sensor_reading[4]);
    }
}

void Dialog::find_arduino_IDinfo(){

    //Function to finds the vendor and product ID for the arduino. This is used to
    //insure other device connected to the computer doesn't get incorrected declared
    //as an arduion.

    //Vendor ID varies by manufactor of Arduino and Product ID will change with product type (Due or Uno)
    //Single usage once info is found

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

}

//*********************
//  Deconstructor
//*********************
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
