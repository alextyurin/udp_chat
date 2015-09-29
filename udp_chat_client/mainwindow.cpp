#include <QTime>
#include <QTimer>
#include<QDebug>
#include "../common/message_interface.hpp"
#include "userlist.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_private_mode(false),
    m_connected(false),
    m_connected_clicked(false)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->general);
    QObject::connect(this->ui->sendButton, SIGNAL(clicked()), this, SLOT(button_clicked()));
    QObject::connect(this->ui->clear, SIGNAL(clicked()), this, SLOT(clear_clicked()));
    QObject::connect(this->ui->actionConnect, SIGNAL(triggered()), this, SLOT(connect_clicked()));
    QObject::connect(this->ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnect_clicked()));
    QObject::connect(this->ui->user_list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(user_list_double_clicked(QModelIndex)));
    QObject::connect(&nClient,SIGNAL(create_client()),this,SLOT(create_user()));
    disable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recieve_msg(const QString &msg)
{
    QString cur_time=QTime::currentTime().toString();
    cur_time.prepend("<font color=grey>");
    cur_time.append("</font> ");
    ui->textView->append(cur_time+msg);
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
    m_connected = true;
    ui->sendButton->setEnabled(true);
    ui->user_list->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->clear->setEnabled(true);
}

void MainWindow::disable()
{
    m_connected = false;
    m_keys.clear();
    ui->user_list->clear();
    ui->sendButton->setEnabled(false);
    ui->user_list->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->clear->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    udp_chat::client::UserList::instance().clear();
}

void MainWindow::button_clicked()
{
    if (ui->textEdit->toPlainText().size() == 0)
    {
        return;
    }
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

void MainWindow::clear_clicked()
{
    ui->textEdit->clear();
    ui->sendButton->setText("Send");
    m_private_mode = false;
}

void MainWindow::connect_clicked()
{
    nClient.show();
}
void MainWindow::create_user()
{
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        m_connected_clicked = true;
        m_server_address = nClient.get_addr();
        m_server_port = nClient.get_port();
        QString nickname;
        nickname = nClient.get_nickname();
//        qDebug()<<m_server_address;
//        qDebug()<<m_server_port;
//        qDebug()<<nickname;
        set_nickname(nickname);
        nClient.hide();
        try_to_connect(m_server_address, m_server_port);
        QTimer *timer = new QTimer(this);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(repeat_connect()));
        timer->start(2000);
}

void MainWindow::repeat_connect()
{
    if (m_connected_clicked)
    {
        if (!m_connected)
        {
            disconnect();
            try_to_connect(m_server_address, m_server_port);
        }
    }
}

void MainWindow::disconnect_clicked()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    m_connected_clicked = false;
    disconnect();
    disable();
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

