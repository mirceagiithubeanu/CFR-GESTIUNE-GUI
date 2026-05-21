#include "ticketwizard.h"
#include "ui_ticketwizard.h"
#include "cfr/cfr.h"
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <vector>
#include <string>
#include <QDebug>
#include <QListWidget>
#include <QAbstractButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QWizard>
#include <QButtonGroup>

TicketWizard::TicketWizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::TicketWizard)
{
    ui->setupUi(this);
    this->setModal(true);

    // sterge tot din wizard initial
    for(int id : this->pageIds())
        this->removePage(id);

    this->addPage(selectStationsPage());
    this->addPage(selectOptionsPage());
    this->addPage(generalInfoPage());
    this->addPage(personalInfoPage());
    this->addPage(fullTicketPage());
    this->addPage(youthTicketPage());
    this->addPage(seniorTicketPage());
    this->addPage(membershipTicketPage());
    this->addPage(finalPage());

    // connect(this->button(QWizard::NextButton), &QAbstractButton::clicked, this, &TicketWizard::nextPage);
    connect(this->button(QWizard::FinishButton), &QAbstractButton::clicked, this, &TicketWizard::buyTicket);
}

QWizardPage* TicketWizard::selectStationsPage() {
    QWizardPage *page = new QWizardPage;

    QLabel *startLabel = new QLabel("Introduceti statia de plecare:");
    QLineEdit *startText = new QLineEdit;
    connect(startText, &QLineEdit::textChanged, this, [this](QString text) {
        start = text;
    });

    QLabel *endLabel = new QLabel("Introduceti statia de sosire:");
    QLineEdit *endText= new QLineEdit;
    connect(endText, &QLineEdit::textChanged, this, [this](QString text) {
        end = text;
    });

    stationInfoBox = new QLabel("");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(startLabel, 0, 0, 1, 1);
    layout->addWidget(startText, 0, 1, 1, 3);

    layout->addWidget(endLabel, 1, 0, 1, 1);
    layout->addWidget(endText, 1, 1, 1, 3);
    layout->addWidget(stationInfoBox, 2, 0, 1, 3);

    page->setLayout(layout);
    return page;
}

QWizardPage* TicketWizard::selectOptionsPage() {
    QWizardPage *page = new QWizardPage;

    QLabel *title = new QLabel("Alegeti o calatorie:");

    QListWidget *list = new QListWidget;
    optionsList = list;
    connect(list, &QListWidget::currentRowChanged, this, &TicketWizard::showOptionInfo);

    QLabel *info = new QLabel("info bilet"); // pt mai tarziu putin
    infoBox = info;

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(list, 1, 0, 4, 1);
    layout->addWidget(info, 1, 2, 4, 3);

    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 3);

    page->setLayout(layout);
    return page;
}

void TicketWizard::showOptionInfo(int index) {
    if(index < 0 || index > (int)options.size())
        return;

    QString info;
    QString startStation = start; // trebuie sa memorez undeva statia de plecare, pentru ca SolEdge nu o memoreaza

    // qDebug() << index;
    option = index;

    if(index >= (int)options.size()) {
        qDebug() << "index prost!";
        return;
    }

    double totalPrice = 0;

    for(auto& opt : options[index]) {
        totalPrice += (opt.arrTime - opt.depTime) / opt.train->getSpeed() * opt.train->getPrice("Vagon Clasa a Doua");

        info += QString::fromStdString(TicketManager::getInstance()->formatTime(opt.depTime) + " - " + TicketManager::getInstance()->formatTime(opt.arrTime));
        info += "\n\t" + startStation + " - " + QString::fromStdString(opt.toStation + "\n\tRuta: " + opt.routeName + "\n");

        startStation = QString::fromStdString(opt.toStation);
    }
    info += "Pret total(la clasa a 2-a): " + QString::number(totalPrice) + " lei\n";

    infoBox->setText(info);
}

void TicketWizard::updateList(QListWidget& list) {
    list.clear();

    int count = 0;
    for(auto& it : options) {
        count++;
        QString depTime, arrTime;
        depTime = QString::fromStdString(TicketManager::getInstance()->formatTime(it.front().depTime));
        arrTime = QString::fromStdString(TicketManager::getInstance()->formatTime(it.back().arrTime));
        list.addItem(QString::number(count) + ": " + depTime + "-" + arrTime);
    }
}

QWizardPage* TicketWizard::generalInfoPage() {
    QWizardPage* page = new QWizardPage();

    QLabel *title = new QLabel("<h3>Informatii generale</h3>");
    QLabel *instr = new QLabel("Pentru fiecare tren din calatorie, alegeti tipul de vagon dorit:\n(Alegere implicita = prima)");

    QListWidget *trainlist = new QListWidget;
    trainsList = trainlist;

    QStackedWidget *selectCar = new QStackedWidget;
    stackedCarSelect = selectCar;
    // widget cu trains;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(instr, 1, 0, 1, 4);
    layout->addWidget(trainlist, 2, 0, 3, 2);
    layout->addWidget(selectCar, 2, 2, 3, 2);
    page->setLayout(layout);

    return page;
}

void TicketWizard::updateGeneralInfo() {
    trainsList->clear();
    carChoice.clear();

    if(stackedCarSelect->count())
        for(int i = stackedCarSelect->count() - 1; i >= 0; i--)
        {
            QWidget* widget = stackedCarSelect->widget(i);
            stackedCarSelect->removeWidget(widget);
            widget->deleteLater();
        }

    QString startSt = start;
    int count = 0;
    for(auto& it : options[option]) {
        count++;

        QString endSt = QString::fromStdString(it.toStation);

        trainsList->addItem("Trenul " + QString::number(count) + ": " + startSt + " - " + endSt);
        carChoice.push_back(0); // creezi vectorul cu alegerile de vagon

        stackedCarSelect->addWidget(traincarSelect(count - 1));

        startSt = QString::fromStdString(it.toStation);
    }

    connect(trainsList, &QListWidget::currentRowChanged, stackedCarSelect, &QStackedWidget::setCurrentIndex);
}

QWidget* TicketWizard::traincarSelect(int trainIndex) {
    QWidget* widget = new QWidget;

    qDebug() << trainIndex;
    Train* train = options[option][trainIndex].train;

    QVBoxLayout *layout = new QVBoxLayout(widget);

    std::vector<std::string> opt = train->getOptions();

    QButtonGroup *buttonGroup = new QButtonGroup(widget);

    int count = 0;
    for(auto& it : opt) {
        count++;
        QRadioButton *radiobutton = new QRadioButton(QString::number(count) + ": " + QString::fromStdString(it));

        connect(radiobutton, &QRadioButton::toggled, this, [this, radiobutton, trainIndex](bool toggled) {
            if(toggled) {
                int index = radiobutton->text().split(":").first().trimmed().toInt() - 1;
                carChoice[trainIndex] = index;
            }
        });

        layout->addWidget(radiobutton);
        buttonGroup->addButton(radiobutton);
    }

    widget->setLayout(layout);
    return widget;
}

QWizardPage* TicketWizard::personalInfoPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h3>Informatii personale client</h3>");
    QLabel *instr = new QLabel("Alegeti tipul de bilet dorit:");

    QListWidget *ticketlist = new QListWidget;
    ticketlist->addItem("1: Bilet Intreg");
    ticketlist->addItem("2: Bilet Tineret");
    ticketlist->addItem("3: Bilet Senior");
    ticketlist->addItem("4: Bilet CFR+");

    connect(ticketlist, &QListWidget::currentRowChanged, this, [this](int row){
        ticketType = row;
    });

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(instr, 1, 0, 3, 2);
    layout->addWidget(ticketlist, 1, 2, 3, 2);
    page->setLayout(layout);

    return page;
}

QWizardPage* TicketWizard::fullTicketPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h4>Infomatii Bilet Intreg</h4>");

    QLabel *nameLabel = new QLabel("Numele:");
    QLineEdit *nameEdit = new QLineEdit;
    connect(nameEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_name = str;
    });

    QLabel *idLabel = new QLabel("CNP:");
    QLineEdit *idEdit = new QLineEdit;
    connect(idEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_id = str;
    });

    QGridLayout *layout = new QGridLayout;
    page->setLayout(layout);
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(nameLabel, 1, 0, 1, 2);
    layout->addWidget(nameEdit, 1, 2, 1, 2);
    layout->addWidget(idLabel, 2, 0, 1, 2);
    layout->addWidget(idEdit, 2, 2, 1, 2);

    return page;
}

QWizardPage* TicketWizard::youthTicketPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h4>Infomatii Bilet Tineret</h4>");

    QLabel *nameLabel = new QLabel("Numele:");
    QLineEdit *nameEdit = new QLineEdit;
    connect(nameEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_name = str;
    });

    QLabel *idLabel = new QLabel("Numarul matricol/ID elev:");
    QLineEdit *idEdit = new QLineEdit;
    connect(idEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_id = str;
    });

    QGridLayout *layout = new QGridLayout;
    page->setLayout(layout);
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(nameLabel, 1, 0, 1, 2);
    layout->addWidget(nameEdit, 1, 2, 1, 2);
    layout->addWidget(idLabel, 2, 0, 1, 2);
    layout->addWidget(idEdit, 2, 2, 1, 2);

    return page;
}

QWizardPage* TicketWizard::seniorTicketPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h4>Infomatii Bilet Senior</h4>");

    QLabel *nameLabel = new QLabel("Numele:");
    QLineEdit *nameEdit = new QLineEdit;
    connect(nameEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_name = str;
    });

    QLabel *idLabel = new QLabel("Codul cardului de sanatate:");
    QLineEdit *idEdit = new QLineEdit;
    connect(idEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_id = str;
    });

    QGridLayout *layout = new QGridLayout;
    page->setLayout(layout);
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(nameLabel, 1, 0, 1, 2);
    layout->addWidget(nameEdit, 1, 2, 1, 2);
    layout->addWidget(idLabel, 2, 0, 1, 2);
    layout->addWidget(idEdit, 2, 2, 1, 2);

    return page;
}

QWizardPage* TicketWizard::membershipTicketPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h4>Infomatii Bilet CFR+</h4>");

    QLabel *nameLabel = new QLabel("Numele:");
    QLineEdit *nameEdit = new QLineEdit;
    connect(nameEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_name = str;
    });

    QLabel *idLabel = new QLabel("CNP:");
    QLineEdit *idEdit = new QLineEdit;
    connect(idEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_id = str;
    });

    QLabel *usrLabel = new QLabel("Numele de utilizator(FARA SPATII):");
    QLineEdit *usrEdit = new QLineEdit;
    connect(usrEdit, &QLineEdit::textChanged, this, [this](QString str){
        c_username = str;
    });

    QGridLayout *layout = new QGridLayout;
    page->setLayout(layout);
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(nameLabel, 1, 0, 1, 2);
    layout->addWidget(nameEdit, 1, 2, 1, 2);
    layout->addWidget(idLabel, 2, 0, 1, 2);
    layout->addWidget(idEdit, 2, 2, 1, 2);
    layout->addWidget(usrLabel, 3, 0, 1, 2);
    layout->addWidget(usrEdit, 3, 2, 1, 2);

    return page;
}

QWizardPage* TicketWizard::finalPage() {
    QWizardPage* page = new QWizardPage;

    QLabel *title = new QLabel("<h4>Datele voastre au fost inregistrate.</h4>");
    QLabel *label2 = new QLabel("Biletul va fi creat si adaugat in sistem odata cu apasarea tastei <i>Finish</i>.");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(label2);
    page->setLayout(layout);

    return page;
}

void TicketWizard::nextPage() {
    switch(this->currentId()) {
        case 1:
            options = TicketManager::getInstance()->getTicketOptionsGUI(start.toStdString(), end.toStdString());
            qDebug() << options.size();

            if(options.empty()) {
                qDebug() << "nu exista ruta!";
                this->reject();
                return;
            }
            if(options[0][0].routeName == "0")
            {
                qDebug() << "statiile date nu exista in sistem!";
                return;
            }

            updateList(*optionsList);
            break;
        case 2:
            updateGeneralInfo();
            break;
        case 4:
            c_name = "";
            c_id = "";
            c_username = "";
            break;
        case 8:
            //buyTicket();
            break;
    }
}

void TicketWizard::buyTicket() {
    TicketManager* inst = TicketManager::getInstance();

    Ticket* ticket = inst->buyTicketGUI(c_name.toStdString(), c_id.toStdString(), c_username.toStdString(), ticketType);

    int count = -1;
    std::string startst = start.toStdString();
    for(auto& it : options[option]) {
        count++;

        std::string dept = inst->formatTime(it.depTime);
        std::string arrt= inst->formatTime(it.arrTime);
        // am: routename, startSt, endSt

        std::string car_type = it.train->getCarOption(carChoice[count]);
        carInfo info = it.train->occupyEmptySeat(car_type, ticket->getId());
        double price = it.train->getPrice(car_type) * (it.arrTime - it.depTime) / it.train->getSpeed();

        inst->createTrip(*ticket, dept, arrt, it.routeName, startst, it.toStation, info.seat_number, info.car_number, car_type, price);

        startst = it.toStation;
    }

    ticket->setTravel();
    ticket->setPrice();

    qDebug() << "Bilet adaugat!";
}

TicketWizard::~TicketWizard()
{
    delete ui;
}

int TicketWizard::nextId() const {
    switch(currentId()) {
        case 3:
            return 4 + ticketType;
        case 4:
        case 5:
        case 6:
        case 7:
            return 8;
        case 8:
            return -1;
        default:
            return QWizard::nextId();
    }
}

bool TicketWizard::validateCurrentPage() {
    qDebug() << currentId();
    switch(currentId()) {
        case 0:
            options = TicketManager::getInstance()->getTicketOptionsGUI(start.toStdString(), end.toStdString());
            qDebug() << options.size();

            if(options.empty()) {
                stationInfoBox->setText("Momentan nu exista nicio ruta disponibila pentru acest traseu.");
                QTimer::singleShot(3500, stationInfoBox, [this](){ this->stationInfoBox->setText(""); });

                //qDebug() << "nu exista ruta!";
                //this->reject();
                return false;
            }
            if(options[0][0].routeName == "0")
            {
                stationInfoBox->setText("Statiile date nu exista in sistem!");
                QTimer::singleShot(3500, stationInfoBox, [this](){ this->stationInfoBox->setText(""); });

                //qDebug() << "statiile date nu exista in sistem!";
                return false;
            }

            updateList(*optionsList);
            return true;
        case 1:
            updateGeneralInfo();
            return true;
        case 3:
            c_name = "";
            c_id = "";
            c_username = "";
            break;
        case 8:
            //buyTicket();
            break;
        case 4:
        case 5:
        case 6:
            if(c_name == "" || c_id == "")
                return false;
            return true;
        case 7:
            if(c_name == "" || c_id == "" || c_username == "")
                return false;
            return true;
    }

    return true;
}
