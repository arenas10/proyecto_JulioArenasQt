#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

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
       void on_velocidad_valueChanged(int value);
       void on_seis_clicked();
       void on_die_clicked();
       void readyToReadData();

       void on_pushButton_3_clicked();


       void on_brix_valueChanged(int arg1);

signals:
       void detector(int x);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    int i,j;
    QString vel,Res,x,b;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
};

#endif // MAINWINDOW_H
