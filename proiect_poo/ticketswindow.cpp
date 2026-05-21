#include "ticketswindow.h"
#include "cfr/cfr.h"
#include "ticketwizard.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QDialog>
#include <QListWidget>

TicketsWindow::TicketsWindow(QWidget *parent)
    : QMainWindow{parent}
{
    QWidget *centralWidget = new QWidget;
    this->setCentralWidget(centralWidget);

    QLabel *title = new QLabel("<h3>Bilete</h3>");
    QLabel *desc = new QLabel("Puteti cumpara bilete, sau vizualiza istoricul cumpararii de bilete.");
    desc->setWordWrap(true);

    QPushButton *buy = new QPushButton("Cumpara Bilet");
    QPushButton *history = new QPushButton("Vizualizeaza Bilete");

    connect(buy, &QPushButton::clicked, this, &TicketsWindow::buyButton_clicked);
    connect(history, &QPushButton::clicked, this, &TicketsWindow::viewButton_clicked);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(desc);
    layout->addWidget(buy);
    layout->addWidget(history);

    centralWidget->setLayout(layout);
}

void TicketsWindow::buyButton_clicked() {
    if(TicketManager::getInstance()->hasTrains())
        buy_ticket_dialog();
    else
        emit give_info("Inca nu exista trenuri in sistem!");

}
void TicketsWindow::viewButton_clicked() {
    if(TicketManager::getInstance()->hasTickets())
        ticketHistoryPage();
    else
        emit give_info("Inca nu a fost cumparat niciun bilet!");
}

void TicketsWindow::buy_ticket_dialog() {
    TicketWizard *wiz = new TicketWizard();
    wiz->setWindowTitle("Cumpararea unui bilet");
    wiz->exec();
    delete wiz;
}

void TicketsWindow::ticketHistoryPage() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Vizualizarea biletelor");

    QLabel *title = new QLabel("Selectati un bilet :");

    QLabel *info = new QLabel("Informatiile vor aparea aici");

    QListWidget *list = new QListWidget;

    qDebug() << "a mers pana la lista";
    std::vector<int> ticketIds = TicketManager::getInstance()->getTickets();

    for(int& it : ticketIds) {
        list->addItem(QString::number(it));
        qDebug() << it;
    }

    connect(list, &QListWidget::currentItemChanged, this, [info](QListWidgetItem *item, QListWidgetItem *prev){
        std::string desc = TicketManager::getInstance()->getTicketInfo(item->text().toInt());
        info->setText(QString::fromStdString(desc));
    });

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(title, 0, 0, 1, 4);
    layout->addWidget(list, 1, 0, 2, 2);
    layout->addWidget(info, 1, 2, 2, 2);

    dialog->setLayout(layout);

    dialog->exec();
}
