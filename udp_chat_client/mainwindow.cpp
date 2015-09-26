#include "../common/message_interface.hpp"
#include "userlist.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_private_mode(false)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->general);
    QObject::connect(this->ui->sendButton, SIGNAL(clicked()), this, SLOT(button_clicked()));
    QObject::connect(this->ui->user_list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(user_list_double_clicked(QModelIndex)));
    disable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recieve_msg(const QString &msg)
{
    ui->textView->append(msg);
}

void MainWindow::show_status(const QString &msg)
{
    ui->statusBar->showMessage(msg);
}

void MainWindow::append_user(const quint32 ip, const quint16 port)
{
    const udp_chat::user_desc_s key(ip, port);
    const QString nickname = udp_chat::client::UserList::instance().get(key);
    m_keys.push_back(key_t(udp_chat::user_desc_s(ip, port), ui->user_list->count()));
    ui->user_list->addItem(nickname);
}

void MainWindow::remove_user(const quint32 ip, const quint16 port)
{
    int row = 0;
    for (auto &key: m_keys)
    {
        if (key.first.ip == ip && key.first.port == port)
        {
            row = key.second;
            m_keys.removeOne(key);
            break;
        }
    }
    ui->user_list->takeItem(row);
}

void MainWindow::enable()
{
    ui->sendButton->setEnabled(true);
    ui->user_list->setEnabled(true);
    ui->textEdit->setEnabled(true);
}

void MainWindow::disable()
{
    m_keys.clear();
    ui->user_list->clear();
    ui->sendButton->setEnabled(false);
    ui->user_list->setEnabled(false);
    ui->textEdit->setEnabled(false);
}

void MainWindow::button_clicked()
{
    if (m_private_mode)
    {
        send_private_msg(m_reciever.ip, m_reciever.port, ui->textEdit->toPlainText().toLocal8Bit());
        ui->sendButton->setText("Send");
        m_private_mode = false;
    }
    else
    {
        send_msg(ui->textEdit->toPlainText().toLocal8Bit());
    }
    ui->textEdit->clear();
}

void MainWindow::user_list_double_clicked(const QModelIndex &index)
{
    udp_chat::user_desc_s user_desc;
    for (auto &key: m_keys)
    {
        if (key.second == static_cast<unsigned int>(index.row()))
        {
            user_desc = key.first;
            break;
        }
    }
    const QString nickname = udp_chat::client::UserList::instance().get(user_desc);
    ui->sendButton->setText("Send to " + nickname);
    m_private_mode = true;
    m_reciever = user_desc;
}

