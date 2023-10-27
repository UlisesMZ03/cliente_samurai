// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qabstractbutton.h"
#include "qlabel.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTcpSocket>
// Estructura de un Samurai
struct Samurai {
    int ID;
    int edad;
    int probabilidadSupervivencia;
    int generacionesEsperadas;
    int inteligenciaEmocional;
    int condicionFisica;
    int fuerzaTroncoSuperior;
    int fuerzaTroncoInferior;
    int resistencia;
};
struct MensajeServidor {
    int codigoRespuesta;
    int fila;
    int columna;
    int fila_back;
    int columna_back;
    int dinero;
    Samurai samurai1;
    Samurai samurai2;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel *labelDinero;
QLabel *labelSamurai1;
    QLabel *labelSamurai2;
private slots:
    void onCellClicked(int row, int column);
    void onButtonClicked(QAbstractButton *button);
    void onButton3Clicked();
private:
    QTableWidget *tableWidget;
    QColor obstacleColor;
    QTcpSocket *clientSocket;
};

#endif // MAINWINDOW_H
