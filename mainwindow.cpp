#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arduinoc();
    ui->groupBoxcaracteristicas->setEnabled(false);
    ui->groupBoxcontrol->setEnabled(false);
    connect(this,SIGNAL(detector(int)),this,SLOT(iniciar()));
    connect(arduino,&QSerialPort::readyRead, this, &MainWindow::readyToReadData);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_iniciar_clicked()
{

    arduino_port_name=ui->comboBox->currentText();


    if((arduino_port_name.indexOf("COM")>=0)||(arduino_port_name.indexOf("tty")>=0)){
        // open and configure the serialport
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "Conectado al puerto "<<arduino_port_name<< "Con 9600 baudios, 8 bits, sin paridad";
         ui->groupBoxcaracteristicas->setEnabled(true);
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Error de puerto", "No se encentra puerto COM o tty disponible");
    }


}

void MainWindow::iniciar()
{

    //variable del motor
       i = ui->velocidad->value();
       //qDebug()<<i;

     if(i<10){
         vel.replace(0,1,"0");
         vel.replace(1,1,QString::number(i));
     }

     else {
         vel=QString::number(i);
     }

     //variable de la resistencia
     j=ui->resistencia->value();
     //qDebug()<<j;

    if(j<10){
         Res.replace(0,1,"0");
         Res.replace(1,1,QString::number(j));
   }

   else {
       Res=QString::number(j);
   }

       //variable del voltaje

    if(ui->seis->isChecked()){
         x="06";
    }

    if(ui->ocho->isChecked()){
         x="08";

    }
    if(ui->die->isChecked()){
        x="10";
    }
    //qDebug()<<x;

    //variable de grados brix
    j=ui->brix->value();
    //qDebug()<<j;
   if(j<10){
        b.replace(0,1,"0");
        b.replace(1,1,QString::number(j));

    }
   else {
       b=QString::number(j);
   }


    if(arduino->isWritable()){
        arduino->write(vel.toUtf8()+Res.toUtf8()+x.toUtf8()+b.toUtf8());
        qDebug()<<QString(vel.toUtf8()+Res.toUtf8()+x.toUtf8()+b.toUtf8());
    }
    else{
        qDebug() << "Couldn't write to serial!";
    }

}

void MainWindow::arduinoc()
{
    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;


    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        ui->comboBox->addItem(serialPortInfo.portName());
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }

}




void MainWindow::on_resistencia_valueChanged(int arg1)
{
    emit detector(arg1);

}


void MainWindow::on_ocho_clicked()
{
    emit detector(1);
}

void MainWindow::on_velocidad_valueChanged(int value)
{
    emit detector(value);   //(2)
}

void MainWindow::on_seis_clicked()
{
    emit detector(3);
}

void MainWindow::on_die_clicked()
{
    emit detector(4);
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->groupBoxcaracteristicas->isEnabled()){
    ui->pushButton_3->setText("Parar");
    ui->groupBoxcontrol->setEnabled(true);
    }

    else{
        ui->pushButton_3->setText("Iniciar");
        ui->groupBoxcontrol->setEnabled(false);
    }

    ui->groupBoxcaracteristicas->setEnabled(!(ui->groupBoxcaracteristicas->isEnabled()));
}

void MainWindow::readyToReadData(){
    char buffer[50];
    while (arduino->canReadLine()){
        arduino->readLine(buffer,50); // lines are already terminated
        ui->temperatura->display(atoi(buffer));
    }
}


void MainWindow::on_brix_valueChanged(int arg1)
{
    emit detector(arg1*300);
}
