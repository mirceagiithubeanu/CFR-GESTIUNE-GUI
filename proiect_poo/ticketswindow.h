#ifndef TICKETSWINDOW_H
#define TICKETSWINDOW_H

#include <QMainWindow>

class TicketsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TicketsWindow(QWidget *parent = nullptr);

signals:
    void give_info(QString info);

private slots:
    void buyButton_clicked();
    void viewButton_clicked();

private:
    void ticketHistoryPage();
    void buy_ticket_dialog();
};

#endif // TICKETSWINDOW_H
