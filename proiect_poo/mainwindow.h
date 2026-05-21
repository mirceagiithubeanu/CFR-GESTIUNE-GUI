#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setStacked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void listWidget_currentRowChanged(int currentRow);
    void show_info(QString info);

private:
    bool busy = false;
    Ui::MainWindow *ui;
    QStackedWidget* stacked;
    QListWidget *list;
    QLabel *infoBox;
};
#endif // MAINWINDOW_H
