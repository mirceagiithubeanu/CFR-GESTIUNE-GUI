#include "traindialog.h"
#include "ui_traindialog.h"
#include "cfr/cfr.h"
#include <QDebug>
#include <QtWidgets>

TrainDialog::TrainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TrainDialog)
{
    ui->setupUi(this);

    title = new QLabel;
    title->setText("Crearea unui tren");

    // cate un widget pentru fiecare etapa(te poti intoarce)
    stacked = new QStackedWidget;

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    backButton = new QPushButton;
    backButton->setText("Back"); // intoarce la widgetul precedent
    connect(backButton, &QPushButton::clicked, this, &TrainDialog::backButton_clicked);

    nextButton = new QPushButton;
    nextButton->setText("Next"); // trece la widgetul urmator
    connect(nextButton, &QPushButton::clicked, this, &TrainDialog::nextButton_clicked);

    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);

    infoBox = new QLabel;

    // acum creezi layout-ul pentru intreg dialogul TrainDialog
    // toate devin copiii dialogului
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(title);
    dialogLayout->addWidget(stacked);
    dialogLayout->addWidget(infoBox);
    dialogLayout->addLayout(buttonLayout);

    //
    // un layout pentru selectarea rutei
    //
    QWidget *wroute = new QWidget; // widget pentru pagina alegerii rutei
    QHBoxLayout *routeLayout = new QHBoxLayout(wroute);

    QLabel *label1 = new QLabel;
    label1->setText("Alegeti o ruta:");
    routeLayout->addWidget(label1);

    // o salvezi pentru mai tarziu:
    routeNames = TicketManager::getInstance()->getRoutes();

    QListWidget *routeList = createList(routeNames);
    routeList->setMaximumWidth(120);
    routeList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    if(routeList->count())
        routeLayout->addWidget(routeList);
    // salvezi indexul si apoi treci la urmatoarea sectiune

    connect(routeList, &QListWidget::currentRowChanged, this, [this](int currentRow){
        routeNo = currentRow;
    });

    stacked->insertWidget(0, wroute);

    //
    // widget pentru a alege tipul de tren
    //
    QWidget *wtrains = new QWidget;
    QHBoxLayout *trainLayout = new QHBoxLayout(wtrains); // layout ul pt label si lista de variante de trenuri

    QLabel *label2 = new QLabel;
    label2->setText("Alegeti un tren:");
    trainLayout->addWidget(label2);

    QListWidget *trainList = new QListWidget;
    trainList->setMaximumWidth(120);
    trainList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    trainList->addItem("Tren de Mare Viteza");
    trainList->addItem("Tren Regional");
    connect(trainList, &QListWidget::currentRowChanged, this, [this](int currentRow){
        trainType = currentRow;
    });
    trainLayout->addWidget(trainList);

    stacked->insertWidget(1, wtrains);

    //
    // introducerea timpului
    //
    QWidget *wtime = new QWidget;
    QVBoxLayout *timeLayout = new QVBoxLayout(wtime); // layout ul pt alegerea timpului

    QLabel *label3 = new QLabel;
    label3->setText("Introduceti timpul de plecare:");
    timeLayout->addWidget(label3);

    QTimeEdit *timeEdit = new QTimeEdit;
    connect(timeEdit, &QTimeEdit::timeChanged, this, [this](const QTime &time) {
        int hours = time.hour();
        int minutes = time.minute();

        depTime = hours * 60 + minutes;
    });
    timeLayout->addWidget(timeEdit);

    stacked->insertWidget(2, wtime);
}

void TrainDialog::backButton_clicked() {
    int nextIndex = stacked->currentIndex() - 1;

    if(nextIndex < 0) {
        this->reject();
        return;
    }

    if(nextIndex == 1) { // a venit de la 2
        nextButton->setText("Next"); // schimbi ca sa scrie din nou next
        infoBox->setText(showInfo(routeNo));
    }
    else // a venit de la 1
        infoBox->setText(showInfo());

    stacked->setCurrentIndex(nextIndex);
}

void TrainDialog::nextButton_clicked() {
    int nextIndex = stacked->currentIndex() + 1;

    if(nextIndex > 2) { // AICI SE TERMINA INTRODUCEREA: APELEZI TICKETMANAGER::ADDTRAIN()
        this->accept();
        bool status = TicketManager::getInstance()->addTrain(routeNames[routeNo], trainType, depTime);

        if(!status)
            qDebug() << "NU S-A ADAUGAT TRENUL!"; // revezi: sa se afiseze un mesaj pentru asta

        return;
    }

    if(nextIndex == 2) { // a venit de la 1
        nextButton->setText("Finish"); // schimbi ca sa scrie din nou next
        infoBox->setText(showInfo(routeNo, trainType));
    }
    else { // a venit de la 0
        infoBox->setText(showInfo(routeNo));
    }

    stacked->setCurrentIndex(nextIndex);
}

QListWidget* TrainDialog::createList(std::vector<std::string> list) {
    QListWidget *qlist = new QListWidget(this);

    for(auto& it : list) {
        qlist->addItem(QString::fromStdString(it));
    }

    return qlist;
}

QString TrainDialog::showInfo(int route, int train) {
    if(route <= -1)
        return "";

    if(train <= -1) {
        return "RUTA ALEASA: " + QString::fromStdString(routeNames[route]);
    }

    QString s;
    s = "RUTA ALEASA: " + QString::fromStdString(routeNames[route]);
    s = s + "\nTIPUL ALES: ";
    if(train == 0)
        s = s + "Tren de Inalta Viteza";
    else
        s = s + "Tren Regional";

    return s;
}


TrainDialog::~TrainDialog()
{
    delete ui;
}
