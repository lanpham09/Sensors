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

    //Variable Initialization
    for (int i = 0; i < 6; ++i){
        arduino_is_available[0] = false;   //initialize port availablity to false
        arduino_port_name[0] == "";        //initalize port name to nothing
        serialBuffer[i] = "";              //initalize serialBuffer to nothing
    }

    //Declaring Arduinos, each arduino is a unique port, currently can't put into array
    arduino[6] = new QSerialPort;

    //****************************************************************************
    //  END OF GLOBAL VARIABLE DEFINITION
    //****************************************************************************

    //Function which determins which ports are connected to a Due. A Due is idenfied
    //by comparing product and vendor ID from the function find_arduino_IDinfo()
    port_assignment();

    //Loop that runs through each avaible arduino found from port_assignment()
    for (int i = 0; i <= number_of_arduinos; ++i) {

        connect_serialport(i);

    }

}//**End of Arduino Constructor

void Dialog::port_assignment(){

    /*Runs though each available port on the computer (include ones not connected to Dues)
     *and what the vendor and product ID of that connection is. Using the information from
     * find_arduino_IDinfo() function the specific product ID and vendor IDs (not more than one)
     * a Due is idenfied. The specific port of that Due is stored into an array and the availablity
     * of that slot number is made true.
     */
    for (int i = 0; i <= number_of_arduinos; ++i) {
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
    }

}//*** End of Port Assignment*********

void Dialog::connect_serialport(int i){
    /*INFO: Connects each avaiable port to a Due*/

    if(arduino_is_available[i]) {
        //if due is available from the port_assignment function than make QSerialPort to connect to
        arduino[i] = new QSerialPort;

        //recommended initalization sequence for arduino configuration
        arduino[i]->setPortName(arduino_port_name[i]);
        arduino[i]->open(QSerialPort::ReadWrite);         //Open the port to send and read data
        arduino[i]->setBaudRate(QSerialPort::Baud9600);
        arduino[i]->setDataBits(QSerialPort::Data8);     //Size of a bite, most modern applications are 8 bit
        arduino[i]->setParity(QSerialPort::NoParity);
        arduino[i]->setStopBits(QSerialPort::OneStop);
        arduino[i]->setFlowControl(QSerialPort::NoFlowControl);

        //connect to arduino and start reading data. From that data red run it through the readSerial
        //slot function. Assigning it to a bool allows for verification of connection.
        bool check_connection = QObject::connect(arduino[i], SIGNAL(readyRead()), this, SLOT(readSerial()));   //function to read data
        //qDebug() << "Check Connection = " << check_connection;
    }
    else {
        //if no port is available then give warning
        QMessageBox::warning(this, "Port Error", "Couldn't find the Arduino for Port!");
    }
}//**End of connect to arduino


void Dialog::readSerial()
    /*Reads data from Due and updates it to the LCD screen*/
{
    for (int i = 0; i <= number_of_arduinos ; ++i){

        if (sender() == arduino[i] ){
            //compares first input to connect and reads data from that specific arduino

            //Reads all data coming from arduino
            serialData[i] = arduino[i]->readAll();
            //takes the data and puts it into a long string one after the other
            serialBuffer[i] += QString::fromStdString(serialData[i].toStdString());

            //splits the serial buffer by the character "," to get individual values
            bufferSplit[i] = serialBuffer[i].split(",");

            if (bufferSplit[i].length() < 6 ){
                //bufferSplit length is very important! if the length is wrong than the data will
                //not print out consistantly. Must be adjust for number of receiving inputs
                //(# of values + 1 for the "" at the end)

                serialData[i] = arduino[i]->readAll();
                serialBuffer[i] += QString::fromStdString(serialData[i].toStdString());

            }
            else {
                //When the bufferSplit has all of the desired values it can be send to updateLCD

                arduino_id[i] = bufferSplit[i][0].toInt();
                qDebug() <<"Arduino " << arduino_id[i] ;
                qDebug() << bufferSplit[i] ;
                Dialog::updateLCD(arduino_id[i], bufferSplit[i]);
                serialBuffer[i] = "" ;         //resets serial buffer so its not infinitely long

            }
        }
    }
}//*** End of Read and Send to LCD Screen


void Dialog::updateLCD(const int arduino_number,const QStringList sensor_reading) {
    //Function to updated LCD screen. Because each LCD screen is it's own variable there isn't a
    //to loop this process

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
    for (int i = 0; i < number_of_arduinos; ++i ){
        if (arduino[i]->isOpen()) {
            arduino[i]->close();
        }
    }
    delete ui;
}//***End of Deconstructor***********************************************
