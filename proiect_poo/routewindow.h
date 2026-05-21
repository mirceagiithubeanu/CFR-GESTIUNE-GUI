#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H

#include <QtWidgets>
#include <vector>
#include <string>
#include <QObject>

class RouteWindow : public QWidget
{
    Q_OBJECT

    QDialog *activeDialog;
    QDialog *routeDialog;
    bool routeAdded = false;

    QStackedWidget *qstacked;
    int routeNr{0}, trainType{0}, depTime{0};

    void add_route(QString text);
    void debugTrain();
    void resetTrain();

    // pentru butoanele NEXT SI BACK

public:
    explicit RouteWindow(QWidget *parent = nullptr);
    ~RouteWindow();

signals:
    void give_info(QString info);

private slots:
    void route_dialog();
    void train_dialog();
    void view_train_system();
};

#endif // ROUTEWINDOW_H
