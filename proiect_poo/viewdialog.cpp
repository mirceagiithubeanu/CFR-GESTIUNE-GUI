#include "viewdialog.h"
#include "cfr/cfr.h"
#include "ui_viewdialog.h"
#include <QtWidgets>
#include <vector>
#include <string>

int parseTime(const QString& time) {
    QStringList parts = time.split(":");
    if(parts.size() != 2)
        return -1;

    bool ok1, ok2;
    int hours = parts[0].toInt(&ok1);
    int minutes = parts[1].toInt(&ok2);

    if(!ok1 || !ok2)
        return -1;

    return hours * 60 + minutes;
}

ViewDialog::ViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewDialog)
{
    ui->setupUi(this);

    QListWidget *list = new QListWidget(this);
    list->addItem("Trenuri");
    list->addItem("Statii");

    QStackedWidget *stacked = new QStackedWidget(this);
    stacked->insertWidget(0, viewTrainsPage());
    stacked->insertWidget(1, viewStationsPage());

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(list);
    layout->addWidget(stacked);

    connect(list, &QListWidget::currentRowChanged, stacked, &QStackedWidget::setCurrentIndex);
}

void ViewDialog::populateTree() {
    std::vector<std::string> routes = TicketManager::getInstance()->getRoutes();
    std::vector<std::string> trains{};

    tree->setColumnCount(1);
    tree->setHeaderLabels({"Rute + Trenuri"});
    QTreeWidgetItem *item;

    for(auto& route : routes) {
        item = new QTreeWidgetItem(tree);
        item->setText(0, QString::fromStdString(route));
        tree->addTopLevelItem(item);

        trains = TicketManager::getInstance()->getTrains(route);
        for(auto& train : trains) {
            QTreeWidgetItem *item2 = new QTreeWidgetItem;
            item2->setText(0, QString::fromStdString(train));

            item->addChild(item2);
        }
    }
}

void ViewDialog::removebutton_clicked() {
    if(tree->topLevelItemCount() == 0) {
        return;
    }

    QTreeWidgetItem *item = tree->currentItem();
    QTreeWidgetItem *itemParent = nullptr;

    if(item->parent() == nullptr) {
        // stergi din SISTEM
        TicketManager::getInstance()->removeRoute(item->text(0).toStdString());
        // stergi din QTREE
        tree->takeTopLevelItem(tree->indexOfTopLevelItem(item));
        return;
    }

    itemParent = item->parent();
    // STERGI DIN SISTEM
    TicketManager::getInstance()->removeTrain(itemParent->text(0).toStdString(), parseTime(item->text(0)));
    // STERGI DIN QTREE
    itemParent->takeChild(itemParent->indexOfChild(item));
}

QWidget* ViewDialog::viewTrainsPage() {
    QWidget* page = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    QTreeWidget *qtree = new QTreeWidget;
    tree = qtree;
    mainLayout->addWidget(qtree);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // butoanele
    QPushButton *backButton = new QPushButton;
    backButton->setText("Back");
    connect(backButton, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton *deleteButton = new QPushButton;
    deleteButton->setText("Remove");
    connect(deleteButton, &QPushButton::clicked, this, &ViewDialog::removebutton_clicked);

    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(deleteButton);

    mainLayout->addLayout(buttonLayout);

    populateTree();
    return page;
}

QWidget* ViewDialog::viewStationsPage() {
    QWidget* page = new QWidget;

    // tree pe mijloc
    // jos buton pentru intoarcere si stergere
    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    QTreeWidget *qqtree = new QTreeWidget;
    mainLayout->addWidget(qqtree);

    // butoanele
    QPushButton *backButton = new QPushButton;
    backButton->setText("Back");
    connect(backButton, &QPushButton::clicked, this, &QDialog::reject);

    mainLayout->addWidget(backButton);

    // populez aici
    std::vector<Route*> rtptr = TicketManager::getInstance()->getRoutePtrs();

    qqtree->setColumnCount(1);
    qqtree->setHeaderLabels({"Rute + Statii"});

    for(auto& route : rtptr) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, QString::fromStdString(route->getRouteName()));
        qqtree->addTopLevelItem(item);

        for(auto& st : route->getStations()) {
            QTreeWidgetItem *item2 = new QTreeWidgetItem;
            item2->setText(0, QString::fromStdString(st));
            item->addChild(item2);
        }
    }
    //

    return page;
}

ViewDialog::~ViewDialog()
{
    delete ui;
}
