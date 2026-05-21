#ifndef TRAINDIALOG_H
#define TRAINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <vector>
#include <string>

namespace Ui {
class TrainDialog;
}

class TrainDialog : public QDialog
{
    Q_OBJECT

    int routeNo{0}, trainType{0}, depTime{0};

    QListWidget* createList(std::vector<std::string> list);
    QString showInfo(int route = -1, int train = -1);

    std::vector<std::string> routeNames;

public:
    explicit TrainDialog(QWidget *parent = nullptr);
    ~TrainDialog();

private slots:
    void nextButton_clicked();
    void backButton_clicked();

private:
    Ui::TrainDialog *ui;
    QLabel *title;
    QStackedWidget *stacked;
    QLabel *infoBox;
    QPushButton *backButton, *nextButton;
};

#endif // TRAINDIALOG_H
