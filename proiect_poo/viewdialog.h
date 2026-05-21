#ifndef VIEWDIALOG_H
#define VIEWDIALOG_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class ViewDialog;
}

class ViewDialog : public QDialog
{
    Q_OBJECT

    void populateTree();

public:
    explicit ViewDialog(QWidget *parent = nullptr);
    ~ViewDialog();

private slots:
    void removebutton_clicked();

private:
    Ui::ViewDialog *ui;
    QTreeWidget *tree;

    QWidget* viewTrainsPage();
    QWidget* viewStationsPage();
};

#endif // VIEWDIALOG_H
