/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *mainContainer;
    QGridLayout *gridLayout_2;
    QScrollArea *chat_field;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label_2;
    QScrollArea *devices_available;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QHBoxLayout *messageBar;
    QLineEdit *messageField;
    QPushButton *messangeSend;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(732, 422);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        mainContainer = new QFrame(centralwidget);
        mainContainer->setObjectName("mainContainer");
        mainContainer->setFrameShape(QFrame::StyledPanel);
        mainContainer->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(mainContainer);
        gridLayout_2->setObjectName("gridLayout_2");
        chat_field = new QScrollArea(mainContainer);
        chat_field->setObjectName("chat_field");
        chat_field->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 486, 350));
        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 10, 101, 21));
        chat_field->setWidget(scrollAreaWidgetContents_2);

        gridLayout_2->addWidget(chat_field, 0, 1, 1, 1);

        devices_available = new QScrollArea(mainContainer);
        devices_available->setObjectName("devices_available");
        devices_available->setMaximumSize(QSize(200, 16777215));
        devices_available->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 198, 382));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 10, 101, 21));
        devices_available->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(devices_available, 0, 0, 2, 1);

        messageBar = new QHBoxLayout();
        messageBar->setObjectName("messageBar");
        messageField = new QLineEdit(mainContainer);
        messageField->setObjectName("messageField");

        messageBar->addWidget(messageField);

        messangeSend = new QPushButton(mainContainer);
        messangeSend->setObjectName("messangeSend");

        messageBar->addWidget(messangeSend);


        gridLayout_2->addLayout(messageBar, 1, 1, 1, 1);


        gridLayout->addWidget(mainContainer, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "chat_field", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "available_devices", nullptr));
        messangeSend->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
