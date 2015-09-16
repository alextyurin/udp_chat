#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->general);
    QObject::connect(this->ui->sendButton, SIGNAL(clicked()), this, SLOT(button_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    ui->sendButton->setEnabled(false);
    ui->textEdit->setEnabled(false);
}

void MainWindow::recieve_msg(const QString &msg)
{
    ui->textView->append(msg);
}

void MainWindow::append_user(const QString &nickname)
{
    ui->user_list->addItem(nickname);
}

void MainWindow::remove_user(const QString &nickname)
{
    for(int i = 0; i < ui->user_list->count(); ++i)
    {
        QListWidgetItem* item = ui->user_list->item(i);
        if (item->text() == nickname)
        {
            ui->user_list->takeItem(ui->user_list->row(item));
        }
    }
}

void MainWindow::button_clicked()
{
    send_msg(ui->textEdit->toPlainText().toLocal8Bit());
    ui->textEdit->clear();
}

