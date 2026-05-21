#include "routewindow.h"
#include "cfr/cfr.h"
#include "traindialog.h"
#include "viewdialog.h"
#include "ticketwizard.h"
#include <QtWidgets>
#include <QtCore>
#include <QDebug>

RouteWindow::RouteWindow(QWidget *parent)
    : QWidget{parent}
{
    // fereastra impartita in: titlu si sectiuna cu optiuni
    QWidget *options = new QWidget;
    QLabel *title = new QLabel("Gestioneaza Rute");
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(title);
    layout->addStretch();
    layout->addWidget(options);
    layout->addStretch();

    // layoutul optiunilor
    QVBoxLayout *optionsLayout= new QVBoxLayout(options);
    optionsLayout->setAlignment(Qt::AlignCenter);

    // BUTOANELE DIN ACEST MENIU
    // ADAUGARE RUTA
    QPushButton *addRoute = new QPushButton;
    addRoute->setText("Adauga ruta");
    addRoute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    addRoute->setFixedWidth(200);
    optionsLayout->addWidget(addRoute);
    connect(addRoute, &QPushButton::clicked, this, &RouteWindow::route_dialog);

    // ADAUGARE TREN
    QPushButton *addTrain = new QPushButton;
    addTrain->setText("Adauga tren");
    addTrain->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    addTrain->setFixedWidth(200);
    optionsLayout->addWidget(addTrain);
    connect(addTrain, &QPushButton::clicked, this, &RouteWindow::train_dialog);

    // VIZUALIZARE RUTE
    QPushButton *deleteRoute = new QPushButton;
    deleteRoute->setText("Vizualizeaza Sistemul de Rute");
    deleteRoute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    deleteRoute->setFixedWidth(200);
    optionsLayout->addWidget(deleteRoute);
    connect(deleteRoute, &QPushButton::clicked, this, &RouteWindow::view_train_system);
}

void RouteWindow::route_dialog() {
    // revezi: fa l sa arate frumos

    QDialog *routeScreen = new QDialog(this);
    routeDialog = routeScreen;
    routeScreen->setModal(true);

    //
    // fereastra impartita in:
    // titlu
    // sectiuna cu optiuni
    //
    QLabel *title = new QLabel();
    title->setText("Introd. ruta(statii separate printr-un spatiu), iar apoi apasati OK.");
    QLineEdit *text = new QLineEdit;

    // optiunile
    QHBoxLayout *optionsLayout = new QHBoxLayout;

    QPushButton *okButton = new QPushButton;
    okButton->setText("OK");
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    okButton->setFixedWidth(200);
    optionsLayout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, [this, text]() {add_route(text->displayText());});

    QPushButton *cancelButton = new QPushButton;
    cancelButton->setText("Cancel");
    cancelButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    cancelButton->setFixedWidth(200);
    optionsLayout->addWidget(cancelButton);
    connect(cancelButton, &QPushButton::clicked, routeScreen, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout(routeScreen);
    layout->setAlignment(Qt::AlignLeft);
    layout->addWidget(title);
    layout->addWidget(text);
    layout->addLayout(optionsLayout);

    routeScreen->exec();
}

void RouteWindow::add_route(QString text) {
    if(text.isEmpty())
        return;

    // ACCESEZI TICKETMANAGER
    TicketManager::getInstance()->addRoute(text.toStdString());
    routeAdded = true;
    routeDialog->accept();
}

void RouteWindow::train_dialog() {
    if(!routeAdded) {
        emit give_info("Inca nu exista rute in sistem!");
        return;
    }

    QDialog *trainDialog = new TrainDialog;

    trainDialog->exec();
    delete trainDialog;
}

void RouteWindow::view_train_system() {
    if(!routeAdded) {
        emit give_info("Inca nu exista rute in sistem!");
        return;
    }

    ViewDialog *view = new ViewDialog;

    view->exec();
    delete view;

    // verifici daca nu cumva nu mai sunt rute din nou
    if(TicketManager::getInstance()->getRoutes().empty())
        routeAdded = false;
}

RouteWindow::~RouteWindow() {
    delete routeDialog;
}
