#ifndef BUYDIALOG_H
#define BUYDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>

namespace Ui {
class BuyDialog;
}

class BuyDialog : public QDialog
{
    Q_OBJECT

    //informatiile pentru client

public:
    explicit BuyDialog(QWidget *parent = nullptr);
    ~BuyDialog();

private:
    Ui::BuyDialog *ui;

    QWizard *wizard;

    //QGraphicsView *view;
    //QGraphicsScene *scene;
};

#endif // BUYDIALOG_H
