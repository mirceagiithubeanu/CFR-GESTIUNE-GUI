#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtCore>
#include <QDebug>
#include <QGridLayout>

#include "routewindow.h"
#include "ticketswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *central = new QWidget;
    this->setCentralWidget(central);
    central->setMinimumWidth(350);

    stacked = new QStackedWidget;
    list = new QListWidget;
    infoBox = new QLabel;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(list, 0, 0, 3, 2);
    layout->addWidget(stacked, 0, 2, 3, 2);
    layout->addWidget(infoBox, 5, 0, 1, 4);

    central->setLayout(layout);

    setStacked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStacked() {
    list->addItem("Start");
    list->addItem("Rute");
    list->addItem("Bilet");
    connect(list, &QListWidget::currentRowChanged, this, &MainWindow::listWidget_currentRowChanged);

    QWidget *startWindow = new QWidget;
    QLabel *label = new QLabel("Start");
    label->setText("Start");
    QLabel *label2 = new QLabel("<h4>Sistem interconenctat de rute si trenuri CFR</h4>");
    QLabel *label3 = new QLabel("Puteti introduce rute si trenuri pe pagina <i>Rute</i>, sau cumpara bilete pe pagina <i>Bilete<\i>");
    QLabel *label4 = new QLabel("Bilet Tineret: reducere 90%\nBilet Senior: reducere 70%\nBilet CFR+: reducere 30%\n");
    label2->setWordWrap(true);
    label3->setWordWrap(true);
    label4->setWordWrap(true);


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(label);
    vlayout->addWidget(label2);
    vlayout->addWidget(label3);
    vlayout->addWidget(label4);
    startWindow->setLayout(vlayout);

    stacked->insertWidget(0, startWindow);

    RouteWindow *routeWindow = new RouteWindow;
    stacked->insertWidget(1, routeWindow);

    TicketsWindow *ticketWindow= new TicketsWindow;
    stacked->insertWidget(2, ticketWindow);

    connect(routeWindow, &RouteWindow::give_info, this, &MainWindow::show_info);
    connect(ticketWindow, &TicketsWindow::give_info, this, &MainWindow::show_info);
}

void MainWindow::listWidget_currentRowChanged(int currentRow)
{
    stacked->setCurrentIndex(currentRow);
}

void MainWindow::show_info(QString info) {
    if(busy)
        return;
    busy = true;

    infoBox->setText(info);
    QTimer::singleShot(3500, infoBox, [this]() {
        infoBox->setText("");

        busy = false;
    });
}
