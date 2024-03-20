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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
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
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *vlayout_message;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *messageBar;
    QLineEdit *messageField;
    QPushButton *messageSend;
    QListView *devices_list;

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
        mainContainer->setMinimumSize(QSize(700, 0));
        mainContainer->setFrameShape(QFrame::StyledPanel);
        mainContainer->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(mainContainer);
        gridLayout_2->setObjectName("gridLayout_2");
        chat_field = new QScrollArea(mainContainer);
        chat_field->setObjectName("chat_field");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chat_field->sizePolicy().hasHeightForWidth());
        chat_field->setSizePolicy(sizePolicy);
        chat_field->setMinimumSize(QSize(480, 0));
        chat_field->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chat_field->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 478, 350));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 0, 1, 1);

        vlayout_message = new QVBoxLayout();
        vlayout_message->setObjectName("vlayout_message");

        gridLayout_3->addLayout(vlayout_message, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 0, 1, 1, 1);

        chat_field->setWidget(scrollAreaWidgetContents_2);

        gridLayout_2->addWidget(chat_field, 0, 1, 1, 1);

        messageBar = new QHBoxLayout();
        messageBar->setObjectName("messageBar");
        messageField = new QLineEdit(mainContainer);
        messageField->setObjectName("messageField");

        messageBar->addWidget(messageField);

        messageSend = new QPushButton(mainContainer);
        messageSend->setObjectName("messageSend");

        messageBar->addWidget(messageSend);


        gridLayout_2->addLayout(messageBar, 1, 1, 1, 1);

        devices_list = new QListView(mainContainer);
        devices_list->setObjectName("devices_list");
        devices_list->setMinimumSize(QSize(210, 0));
        devices_list->setMaximumSize(QSize(250, 16777215));

        gridLayout_2->addWidget(devices_list, 0, 0, 2, 1);


        gridLayout->addWidget(mainContainer, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        messageSend->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
