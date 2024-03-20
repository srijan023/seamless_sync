#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QLabel>
#include<QScrollBar>
#include<QStringListModel>
#include<QScrollArea>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_messageSend_clicked()
{
    QString message= ui->messageField->text();
    QLabel *new_message = new QLabel(message);
    QScrollBar *chatScroll = ui->chat_field->verticalScrollBar();
    // QLabel *from_other_device=new QLabel("Hey receiver");
    // from_other_device->setStyleSheet("border: 1px solid black; border-radius: 5px; width: fit-content; padding:4px;");
    // ui->vlayout_received->addWidget(from_other_device);
    // from_other_device->setAlignment(Qt::AlignLeft);
    // QWidget *messageContainer = new QWidget();
    // messageContainer->setLayout(ui->vlayout_message);
    // ui->chat_field->setWidget(messageContainer);
    // QWidget *showing_message= new_message;

    new_message->setStyleSheet("border: 1px solid black; border-radius: 5px; width: fit-content; padding:4px; text-align: right;");
    new_message->setAlignment(Qt::AlignRight);
    new_message->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


    if(!message.isEmpty()){

        int lineLength = 50;  // Change this to your desired line length
        // Insert newline characters every 'lineLength' characters
        QString wrappedText;
        for(int i = 0; i < message.length(); i++) {
            if(i % lineLength == 0 && i != 0)
                wrappedText.append('\n');
            wrappedText.append(message[i]);
        }
        // Set the wrapped text to the label
        new_message->setText(wrappedText);
        ui->vlayout_message->addWidget(new_message);
        // After adding a new message widget to the messageLayout:
        chatScroll->setValue(chatScroll->SliderToMaximum);
        ui->messageField->clear();
        ui->messageField->setFocus();
    }


}


// void MainWindow::on_messageSend_clicked()
// {
//     QString message=ui->messageField->text();
//     QLabel *new_message= new QLabel(message);
//     QWidget *container = new QWidget();
//     QVBoxLayout *chats = new QVBoxLayout();
//     ui->chat_field->setWidget(container);
//     new_message->setStyleSheet("border: 1px solid black; border-radius: 5px; width: fit-content; padding:4px; text-align: right;");



// }

