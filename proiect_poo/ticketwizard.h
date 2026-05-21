#ifndef TICKETWIZARD_H
#define TICKETWIZARD_H

#include <QWizard>
#include <QtCore>
#include <vector>
#include <string>
#include <cfr/cfr.h>
#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>

namespace Ui {
class TicketWizard;
}

class TicketWizard : public QWizard
{
    Q_OBJECT

public:
    explicit TicketWizard(QWidget *parent = nullptr);
    ~TicketWizard();

private slots:
    void nextPage();

private:
    Ui::TicketWizard *ui;

    QListWidget *optionsList = nullptr;
    QLabel *infoBox = nullptr;
    QListWidget *trainsList = nullptr;
    QLabel *cars = nullptr;
    QStackedWidget *stackedCarSelect = nullptr;
    QLabel *stationInfoBox = nullptr, *personalInfoBox = nullptr;

    int option = 0;

    // datele unui bilet
    QString start, end; // statiile
    QString c_name, c_id, c_username;
    int ticketType = 0;
    std::vector<std::vector<SolEdge>> options;
    // vector cu atatea elemente cat alegerea din options; este creat dupa ce se alege o varianta din options
    std::vector<int> carChoice;

    QWizardPage* selectStationsPage();
    QWizardPage* selectOptionsPage();
    QWizardPage* generalInfoPage();
    QWizardPage* personalInfoPage();
    QWizardPage* fullTicketPage();
    QWizardPage* youthTicketPage();
    QWizardPage* seniorTicketPage();
    QWizardPage* membershipTicketPage();
    QWizardPage* finalPage();

    QWidget* traincarSelect(int trainIndex);

    void updateList(QListWidget& list);
    void showOptionInfo(int index);
    void updateGeneralInfo();
    void buyTicket();

    int nextId() const override;
    bool validateCurrentPage() override;
};

#endif // TICKETWIZARD_H
