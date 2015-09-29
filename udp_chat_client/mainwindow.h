#pragma once

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QPair>
#include "../common/message_interface.hpp"
#include "client.hpp"
#include"client_con.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    typedef QPair<udp_chat::user_desc_s, quint32> key_t;
    typedef QList<key_t> keys_t;
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    client_con nClient;
signals:
    void send_msg(const QString &msg);
    void send_private_msg(const quint32 ip, const quint16 port, const QString &msg);
    void try_to_connect(const QHostAddress &server_address, const quint16 port);
    void disconnect();
    void set_nickname(const QString &nickname);
public slots:
    void repeat_connect();
    void recieve_msg(const QString &msg);
    void show_status(const QString &msg);
    void append_user(const quint32 ip, const quint16 port);
    void remove_user(const quint32 ip, const quint16 port);
    void enable();
    void disable();
private slots:
    void button_clicked();
    void clear_clicked();
    void create_user();
    void connect_clicked();
    void disconnect_clicked();
    void user_list_double_clicked(const QModelIndex &index);
private:

    keys_t m_keys;
    Ui::MainWindow *ui;
    QHostAddress m_server_address;
    quint16 m_server_port;
    bool m_private_mode;
    udp_chat::user_desc_s m_reciever;
    bool m_connected;
    bool m_connected_clicked;
};

