/********************************************************************************
** Form generated from reading UI file 'traindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINDIALOG_H
#define UI_TRAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_TrainDialog
{
public:

    void setupUi(QDialog *TrainDialog)
    {
        if (TrainDialog->objectName().isEmpty())
            TrainDialog->setObjectName(QString::fromUtf8("TrainDialog"));
        TrainDialog->resize(400, 300);

        retranslateUi(TrainDialog);

        QMetaObject::connectSlotsByName(TrainDialog);
    } // setupUi

    void retranslateUi(QDialog *TrainDialog)
    {
        TrainDialog->setWindowTitle(QCoreApplication::translate("TrainDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainDialog: public Ui_TrainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINDIALOG_H
