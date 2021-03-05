#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
       void on_iniciar_clicked();
       void arduinoc();
       void iniciar();
       void on_ocho_clicked();
       void on_resistencia_valueChanged(int arg1);
       //void on_velocidad_valueChanged(int value);
       void on_seis_clicked();
       void on_die_clicked();
       void readyToReadData();
       void updateTime();

       void on_pushButton_3_clicked();


       void on_brix_valueChanged(int arg1);

       void on_botonON_clicked();

       void on_botonOFF_clicked();

       void on_cero_clicked();

       void on_pushButton_clicked();

       void on_pushButton_2_clicked();

signals:
       void detector(int x);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    int i=0;
    int j;
    ulong tiempoS=0;
    bool escriturahabilitada=0;
    QString vel,Res,x,b;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QTime tiempoTranscurrido,tiempoInicio,tiempoActual;
    QDateTime fecha;
    int tiempotimer=0;
    QTimer timer;
    QTextStream outstream;
    QFile myFile;
    QString nombreArchivo;
};

#endif // MAINWINDOW_H
