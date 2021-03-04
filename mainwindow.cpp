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
    tiempotimer=10000;//10 segundos
    ui->setupUi(this);
    arduinoc();
    ui->groupBoxcaracteristicas->setEnabled(false);
    ui->groupBoxcontrol->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->pushButton->setStyleSheet("QPushButton{font-size: 18px;font-family: Arial;color: rgb(0, 0, 0);background-color: rgb(204,0,0);}");
    ui->temperatura->setStyleSheet("QLCDNumber{color: rgb(254, 0, 0);}");
    connect(this,SIGNAL(detector(int)),this,SLOT(iniciar()));
    connect(arduino,&QSerialPort::readyRead, this, &MainWindow::readyToReadData);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));


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
         ui->groupBox->setEnabled(true);
         ui->groupBoxcontrol->setEnabled(true);
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Error de puerto", "No se encuentra puerto COM o tty disponible\nRevisa conexiones y pulsa OK");
        arduinoc();
    }


}

void MainWindow::iniciar()
{

    //variable del motor

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
    if(ui->cero->isChecked()){
        x="00";
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
    if(escriturahabilitada){
        if(arduino->isWritable()){
            arduino->write(vel.toUtf8()+Res.toUtf8()+x.toUtf8()+b.toUtf8());
            qDebug()<<QString(vel.toUtf8()+Res.toUtf8()+x.toUtf8()+b.toUtf8());
        }
        else{
            qDebug() << "Couldn't write to serial!";
        }
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
/*
void MainWindow::on_velocidad_valueChanged(int value)
{
    emit detector(value);   //(2)
}*/

void MainWindow::on_seis_clicked()
{
    emit detector(3);
}

void MainWindow::on_die_clicked()
{
    emit detector(4);
}

//https://hetpro-store.com/TUTORIALES/qfile-para-crear-archivos-en-qt-creator/
void MainWindow::on_pushButton_3_clicked()

{
    if(ui->groupBoxcaracteristicas->isEnabled()){//al cliquear en iniciar
    ui->pushButton_3->setText("Finalizar");
    ui->groupBoxcontrol->setEnabled(true);
    escriturahabilitada=1;
    iniciar();
    tiempoTranscurrido.setHMS(0,0,0,0);
    tiempoS=0;
    ui->tiempominutos->display(0);
    timer.start(tiempotimer);
    myFile.setFileName(ui->route->text()+ui->filename->text()+".csv");
    if(myFile.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug("Abre correctamente");
    outstream.setDevice(&myFile);
    outstream <<ui->fruta->text()<<endl;
    if(ui->frutaMadura->isChecked())
        outstream << "Madura"<<endl;
    else if (ui->frutaPinton->isChecked()) {
        outstream << "Pinton"<<endl;
    }
    else if (ui->frutaVerde->isChecked()) {
        outstream << "Verde"<<endl;
    }

    QString FORMAT = "d/MM/yyyy hh:mm:ss";
    outstream<<QDateTime::currentDateTime().toString(FORMAT)<<endl;
    qDebug()<<QDateTime::currentDateTime().toString(FORMAT);
    outstream<<"Tiempo (s),Motor,Resistencia,Barras (V),GradosBrix,Temperatura(C)"<<endl;
    }

    else{ //al cliquear en parar
        ui->pushButton_3->setText("Iniciar transmisión");
        ui->resistencia->setValue(0);
        ui->brix->setValue(0);
        i=11;
        ui->cero->setChecked(true);
        ui->botonON->setEnabled(true);
        ui->botonOFF->setEnabled(true);
        iniciar();
        escriturahabilitada=0;
        timer.stop();
        ui->tiempominutos->display(0);
        myFile.close();
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

void MainWindow::on_botonON_clicked()
{
    i=99;
    ui->botonON->setEnabled(false);
    ui->botonOFF->setEnabled(true);
    iniciar();
}

void MainWindow::on_botonOFF_clicked()
{
    i=11;
    ui->botonOFF->setEnabled(false);
    ui->botonON->setEnabled(true);
    iniciar();
}

void MainWindow::on_cero_clicked()
{
    emit detector(8);
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
void MainWindow::updateTime(){
    if(escriturahabilitada){
        tiempoTranscurrido=tiempoTranscurrido.addMSecs(tiempotimer);
        tiempoS=tiempoS+(tiempotimer/1000);
        outstream<<QString::number(tiempoS)<<',';
        if(i==99)
            outstream<<"1,";//escribir motor
        else {
            outstream<<"0,";
        }

        outstream<<ui->resistencia->text()<<',';//escribir resistencia

        if(ui->cero->isChecked())//escribir Barras
            outstream<<"0,";
        else if (ui->seis->isChecked()) {
            outstream<<"6,";
        }
        else if (ui->ocho->isChecked()) {
            outstream<<"8,";
        }
        else if (ui->die->isChecked()) {
            outstream<<"10,";
        }
        outstream<<QString::number(ui->brix->value())<<','<<QString::number(ui->temperatura->value())<<endl;

    }
    else {    tiempoTranscurrido.setHMS(0,0,0,0);
        tiempoS=0;
    }
    qDebug("tiempoTranscurrido %d segundos",tiempoTranscurrido.second());
    qDebug("tiempoTranscurrido %d min",tiempoTranscurrido.minute());
    ui->tiempominutos->display(int(tiempoTranscurrido.minute()));


}
