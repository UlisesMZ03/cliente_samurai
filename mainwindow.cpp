// mainwindow.cpp

#include "mainwindow.h"
#include "qradiobutton.h"
#include <QTableWidgetItem>
#include <ostream>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
int obstaculo_elegido = 71;
int prev_pos_x=0;
int prev_pos_y=0 ;
int filaRespuesta = 0;
int columnaRespuesta = 0;
int filaRespuesta_back = 0;
int columnaRespuesta_back = 0;
int dinero=10;
Samurai samurai1;
Samurai samurai2;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Configurar la ventana principal
    setWindowTitle("Samurai Stronghold");
    resize(1280, 720);
    // Crear un nuevo widget como contenedor principal
    QWidget *mainWidget = new QWidget(this);

    // Calcular el ancho de la ventana
    int windowWidth = this->width();

    // Configurar la tabla como 10x10 y establecer su ancho como la mitad del ancho de la ventana
    tableWidget = new QTableWidget(10, 10, mainWidget);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    int tableWidth = windowWidth / 2;
    tableWidget->setMinimumWidth(tableWidth);
    tableWidget->setMaximumWidth(tableWidth);

    // Hacer las celdas cuadradas y no editables
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        tableWidget->setRowHeight(i, 60); // Establecer la altura de la fila
        tableWidget->setColumnWidth(i, 60); // Establecer el ancho de la columna
        for (int j = 0; j < tableWidget->columnCount(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(item->flags() ^ Qt::ItemIsEditable); // Hacer la celda no editable
            tableWidget->setItem(i, j, item);
        }
    }

    // Conectar la señal de clic de la celda al slot correspondiente
    connect(tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onCellClicked(int,int)));




    // Color de fondo predeterminado para las casillas marcadas como obstáculos
    obstacleColor = Qt::black; // Puedes cambiar el color de los obstáculos aquí

    // Crear un QLabel para mostrar el dinero
    labelDinero = new QLabel("Dinero: " + QString::number(dinero), mainWidget);
    // Crear un QLabel para mostrar el dinero
    labelSamurai1 = new QLabel("Resistencia A*: " + QString::number(samurai1.resistencia), mainWidget);
    // Crear un diseño vertical y agregar la tabla y el botón a ese diseño
    labelSamurai2 = new QLabel("Resistencia Backtracking: " + QString::number(samurai2.resistencia), mainWidget);
    // Crear un diseño vertical y agregar la tabla y el botón a ese diseño
    QHBoxLayout *layout = new QHBoxLayout(mainWidget);
    layout->addWidget(tableWidget);

    QVBoxLayout *buttonsLayout = new QVBoxLayout(mainWidget);

    QGridLayout *contenedor_derecha = new QGridLayout(mainWidget);
    layout->addLayout(buttonsLayout);
    contenedor_derecha->addWidget(labelSamurai1);
    contenedor_derecha->addWidget(labelSamurai2);
    contenedor_derecha->addWidget(labelDinero); // Agregar el QLabel al diseño

    QRadioButton *button = new QRadioButton("Yari", mainWidget);
    QRadioButton *button1 = new QRadioButton("Arco y flechas", mainWidget);
    QRadioButton *button2 = new QRadioButton("Tanegashima", mainWidget);

    QPushButton *button3 = new QPushButton("Next", mainWidget);
    connect(button3, SIGNAL(clicked()), this, SLOT(onButton3Clicked()));


    button->setChecked(true); // Seleccionar el botón "Yari" por defecto

        // Crear un grupo de botones de radio para asegurar la selección mutua
    QButtonGroup *buttonGroup = new QButtonGroup(mainWidget);
    buttonGroup->addButton(button);
    buttonGroup->addButton(button1);
    buttonGroup->addButton(button2);


    // Conectar la señal de selección del botón de radio a un slot correspondiente
    connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));

    buttonsLayout->addWidget(button);
    buttonsLayout->addWidget(button1);
    buttonsLayout->addWidget(button2);
    buttonsLayout->addWidget(button3);
    layout->addLayout(contenedor_derecha);

    // Establecer el diseño como el diseño principal del contenedor
    mainWidget->setLayout(layout);

    // Establecer el contenedor como el widget central de la ventana principal
    setCentralWidget(mainWidget);


    // Crear el socket del cliente y conectar al servidor
    clientSocket = new QTcpSocket(this);
    clientSocket->connectToHost("127.0.0.1", 12345); // Dirección IP y puerto del servidor

    // Conectar señal de error para manejar posibles errores de conexión
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(leerDatosDelServidor()));


    // Verificar la conexión con el servidor
    if (clientSocket->waitForConnected()) {
        std::cout << "Conectado al servidor." << std::endl;
    } else {
        std::cerr << "Error al conectar al servidor." << std::endl;
    }

}

MainWindow::~MainWindow()
{
}


void MainWindow::onButton3Clicked()
{
    labelDinero->setText("Dinero: " + QString::number(dinero));
    // Actualizar el saldo de dinero

    labelDinero->setText("Dinero: " + QString::number(dinero));
    labelSamurai1->setText("Resistencia A*: " + QString::number(samurai1.resistencia));
    labelSamurai2->setText("Resistencia Backtracking: " + QString::number(samurai2.resistencia));
    std::cout << "dinero: " << dinero << std::endl;
    // Cambiar el color de fondo de la celda al ser clicada
    QTableWidgetItem *item = tableWidget->item(0, 0);
    // Enviar los valores de fila y columna al servidor
    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        // Enviar fila y columna al servidor
        MensajeServidor mensajeCliente;
        mensajeCliente.codigoRespuesta = 0;
        mensajeCliente.fila = 0;
        mensajeCliente.columna = 0;
        send(clientSocket->socketDescriptor(), &mensajeCliente, sizeof(mensajeCliente), 0);


        prev_pos_y=filaRespuesta;
        prev_pos_x=columnaRespuesta;
        int prev_pos_y_b=filaRespuesta_back;
        int prev_pos_x_b=columnaRespuesta_back;
        // Recibir la respuesta del servidor
        MensajeServidor mensajeServidor;
        clientSocket->read(reinterpret_cast<char*>(&mensajeServidor), sizeof(mensajeServidor));

        // Procesar la respuesta del servidor
        int codigoRespuesta = mensajeServidor.codigoRespuesta;


        // Realizar acciones según el código de respuesta del servidor
        if (codigoRespuesta == 200) {
            filaRespuesta = mensajeServidor.fila;
            columnaRespuesta = mensajeServidor.columna;
            filaRespuesta_back = mensajeServidor.fila_back;
            columnaRespuesta_back = mensajeServidor.columna_back;
            dinero = mensajeServidor.dinero;
            samurai1 = mensajeServidor.samurai1;
            samurai2 = mensajeServidor.samurai2;

            std::cout << "Servidor respondiiiió: Fila " << dinero << ", Columna " << columnaRespuesta << std::endl;
                    // Pintar la casilla en la fila y columna recibidas del servidor
                    QTableWidgetItem *item = tableWidget->item(filaRespuesta, columnaRespuesta);
            QTableWidgetItem *prev_item = tableWidget->item(prev_pos_y, prev_pos_x);
            QTableWidgetItem *item_back = tableWidget->item(filaRespuesta_back, columnaRespuesta_back);
            QTableWidgetItem *prev_item_back = tableWidget->item(prev_pos_y_b, prev_pos_x_b);
            if (item_back) {
                prev_item_back->setBackgroundColor(Qt::white); // Cambiar el color de fondo de la celda
            }
            if (item_back) {
                item_back->setBackgroundColor(Qt::red); // Cambiar el color de fondo de la celda
            }
            if (item) {
                prev_item->setBackgroundColor(Qt::white); // Cambiar el color de fondo de la celda
            }
            if (item) {
                item->setBackgroundColor(Qt::green); // Cambiar el color de fondo de la celda
            } else {
                std::cerr << "Error: La celda en la fila " << filaRespuesta << ", columna " << columnaRespuesta << " no existe." << std::endl;
            }
            // Hacer algo con la respuesta del servidor, si es necesario
        }
        else {
            std::cerr << "Código de respuesta desconocido: " << codigoRespuesta << std::endl;
            // Manejar otros códigos de respuesta, si es necesario
        }
    } else {
        std::cerr << "Error: No hay conexión con el servidor." << std::endl;
    }
}

void MainWindow::onButtonClicked(QAbstractButton *button)
{

    // Verificar qué botón de radio ha sido seleccionado
    if (button->text() == "Yari") {
        obstaculo_elegido = 71; // Asignar el valor correspondiente para el botón "Yari"
    } else if (button->text() == "Arco y flechas") {
        obstaculo_elegido = 72; // Asignar el valor correspondiente para el botón "Arco y flechas"
    } else if (button->text() == "Tanegashima") {
        obstaculo_elegido = 73; // Asignar el valor correspondiente para el botón "Tanegashima"
    }
}

void MainWindow::onCellClicked(int row, int column)
{
    if ((row == 0 && column == 0) || (row == 9 && column == 9)) {
        // No hacer nada si la celda es la esquina superior izquierda o la esquina inferior derecha

        return;
    }
    // Obtener el costo del obstáculo seleccionado
    int costoObstaculo = 0;
    if (obstaculo_elegido == 71) {
        costoObstaculo = 1;
    } else if (obstaculo_elegido == 72) {
        costoObstaculo = 2;
    } else if (obstaculo_elegido == 73) {
        costoObstaculo = 4;
    }

    // Verificar si tienes suficiente dinero para el obstáculo
    if (dinero >= costoObstaculo) {
        labelDinero->setText("Dinero: " + QString::number(dinero));
        // Actualizar el saldo de dinero
        dinero -= costoObstaculo;
        labelDinero->setText("Dinero: " + QString::number(dinero));
        labelSamurai1->setText("Resistencia A*: " + QString::number(samurai1.resistencia));
        labelSamurai2->setText("Resistencia Backtracking: " + QString::number(samurai2.resistencia));
        std::cout << "dinero: " << dinero << std::endl;
        // Cambiar el color de fondo de la celda al ser clicada
        QTableWidgetItem *item = tableWidget->item(row, column);
        // Enviar los valores de fila y columna al servidor
        std::cout << "Fila: " << row << ", Columna: " << column << std::endl;
        if (clientSocket->state() == QAbstractSocket::ConnectedState) {
            // Enviar fila y columna al servidor
            MensajeServidor mensajeCliente;
            mensajeCliente.codigoRespuesta = obstaculo_elegido;
            mensajeCliente.fila = row;
            mensajeCliente.columna = column;
            send(clientSocket->socketDescriptor(), &mensajeCliente, sizeof(mensajeCliente), 0);


            prev_pos_y=filaRespuesta;
            prev_pos_x=columnaRespuesta;
            int prev_pos_y_b=filaRespuesta_back;
            int prev_pos_x_b=columnaRespuesta_back;
            // Recibir la respuesta del servidor
            MensajeServidor mensajeServidor;
            clientSocket->read(reinterpret_cast<char*>(&mensajeServidor), sizeof(mensajeServidor));

            // Procesar la respuesta del servidor
            int codigoRespuesta = mensajeServidor.codigoRespuesta;


            // Realizar acciones según el código de respuesta del servidor
            if (codigoRespuesta == 200) {
                filaRespuesta = mensajeServidor.fila;
                columnaRespuesta = mensajeServidor.columna;
                filaRespuesta_back = mensajeServidor.fila_back;
                columnaRespuesta_back = mensajeServidor.columna_back;
                dinero = mensajeServidor.dinero;
                samurai1 = mensajeServidor.samurai1;
                samurai2 = mensajeServidor.samurai2;

                std::cout << "Servidor respondiiiió: Fila " << dinero << ", Columna " << columnaRespuesta << std::endl;
                        // Pintar la casilla en la fila y columna recibidas del servidor
                QTableWidgetItem *item = tableWidget->item(filaRespuesta, columnaRespuesta);
                QTableWidgetItem *prev_item = tableWidget->item(prev_pos_y, prev_pos_x);
                QTableWidgetItem *item_back = tableWidget->item(filaRespuesta_back, columnaRespuesta_back);
                QTableWidgetItem *prev_item_back = tableWidget->item(prev_pos_y_b, prev_pos_x_b);
                if (item_back) {
                    prev_item_back->setBackgroundColor(Qt::white); // Cambiar el color de fondo de la celda
                }
                if (item_back) {
                    item_back->setBackgroundColor(Qt::red); // Cambiar el color de fondo de la celda
                }
                if (item) {
                prev_item->setBackgroundColor(Qt::white); // Cambiar el color de fondo de la celda
                }
                if (item) {
                    item->setBackgroundColor(Qt::green); // Cambiar el color de fondo de la celda
                } else {
                    std::cerr << "Error: La celda en la fila " << filaRespuesta << ", columna " << columnaRespuesta << " no existe." << std::endl;
                }
                // Hacer algo con la respuesta del servidor, si es necesario
            }
            else {
                std::cerr << "Código de respuesta desconocido: " << codigoRespuesta << std::endl;
                // Manejar otros códigos de respuesta, si es necesario
            }
        } else {
            std::cerr << "Error: No hay conexión con el servidor." << std::endl;
        }

        if (item) {
            // Si la celda está marcada como obstáculo, quitar el obstáculo
            if (item->backgroundColor() == obstacleColor) {
                item->setBackgroundColor(Qt::white); // Color de fondo predeterminado
            } else {
                if (obstaculo_elegido==71){
                    item->setBackgroundColor(Qt::yellow);
                }
                else if(obstaculo_elegido==72){
                    item->setBackgroundColor(Qt::blue);
                }
                else if(obstaculo_elegido==73){
                    item->setBackgroundColor(Qt::black);
                }
                // Marcar la celda como obstáculo
            }
        }
    }
}
