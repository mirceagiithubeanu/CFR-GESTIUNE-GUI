#include "buydialog.h"
#include "ui_buydialog.h"
#include <QtCore>
#include <QtWidgets>

BuyDialog::BuyDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuyDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Cumpararea unui bilet");

    wizard = new QWizard(this);
    wizard->show();
    /*
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    view->setScene(scene);

    QBrush brush(Qt::lightGray);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    QGraphicsEllipseItem *ellipse1 = scene->addEllipse(10, 0, 50, 50, blackPen, brush);
    QGraphicsEllipseItem *ellipse2 = scene->addEllipse(10, -100, 50, 50, blackPen, brush);
    QGraphicsEllipseItem *ellipse3 = scene->addEllipse(10, -200, 50, 50, blackPen, brush);
    QGraphicsEllipseItem *ellipse4 = scene->addEllipse(10, -300, 50, 50, blackPen, brush);

    QGraphicsLineItem *line1 = scene->addLine(10, 10, 10, -90, blackPen);
    QGraphicsLineItem *line2 = scene->addLine(10, -90, 10, -190, blackPen);
    */
}

BuyDialog::~BuyDialog()
{
    delete ui;
}
