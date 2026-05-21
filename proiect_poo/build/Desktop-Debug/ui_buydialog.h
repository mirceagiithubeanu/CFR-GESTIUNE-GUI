/********************************************************************************
** Form generated from reading UI file 'buydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUYDIALOG_H
#define UI_BUYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_BuyDialog
{
public:

    void setupUi(QDialog *BuyDialog)
    {
        if (BuyDialog->objectName().isEmpty())
            BuyDialog->setObjectName(QString::fromUtf8("BuyDialog"));
        BuyDialog->resize(400, 300);

        retranslateUi(BuyDialog);

        QMetaObject::connectSlotsByName(BuyDialog);
    } // setupUi

    void retranslateUi(QDialog *BuyDialog)
    {
        BuyDialog->setWindowTitle(QCoreApplication::translate("BuyDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuyDialog: public Ui_BuyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUYDIALOG_H
