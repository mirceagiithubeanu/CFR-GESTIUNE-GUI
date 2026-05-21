/********************************************************************************
** Form generated from reading UI file 'ticketwizard.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETWIZARD_H
#define UI_TICKETWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_TicketWizard
{
public:
    QWizardPage *wizardPage1;
    QWizardPage *wizardPage2;

    void setupUi(QWizard *TicketWizard)
    {
        if (TicketWizard->objectName().isEmpty())
            TicketWizard->setObjectName(QString::fromUtf8("TicketWizard"));
        TicketWizard->resize(400, 300);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QString::fromUtf8("wizardPage1"));
        TicketWizard->addPage(wizardPage1);
        wizardPage2 = new QWizardPage();
        wizardPage2->setObjectName(QString::fromUtf8("wizardPage2"));
        TicketWizard->addPage(wizardPage2);

        retranslateUi(TicketWizard);

        QMetaObject::connectSlotsByName(TicketWizard);
    } // setupUi

    void retranslateUi(QWizard *TicketWizard)
    {
        TicketWizard->setWindowTitle(QCoreApplication::translate("TicketWizard", "Wizard", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TicketWizard: public Ui_TicketWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETWIZARD_H
