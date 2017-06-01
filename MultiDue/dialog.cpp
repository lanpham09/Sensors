#include "dialog.h"
#include "ui_dialog.h"

#include "arduino.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QWidget>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //***Display Setup******************************
    ui->Post_lcdNumber->display("-------");
    ui->Hor_lcdNumber->display("-------");
    ui->Ver_lcdNumber->display("-------");
    ui->Omni_lcdNumber->display("-------");
    ui->Post_lcdNumber_2->display("-------");
    ui->Hor_lcdNumber_2->display("-------");
    ui->Ver_lcdNumber_2->display("-------");
    ui->Omni_lcdNumber_2->display("-------");
    //***End Display Setup**************************

    Arduino();

}

Dialog::~Dialog()
{
    delete ui;
}
