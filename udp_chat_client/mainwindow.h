#pragma once

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QPair>
#include "../common/message_interface.hpp"
#include "client.hpp"

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
signals:
    void send_msg(const QString &msg);
    void send_private_msg(const quint32 ip, const quint16 port, const QString &msg);
public slots:
    void recieve_msg(const QString &msg);
    void show_status(const QString &msg);
    void append_user(const quint32 ip, const quint16 port);
    void remove_user(const quint32 ip, const quint16 port);
    void enable();
    void disable();
private slots:
    void button_clicked();
    void user_list_double_clicked(const QModelIndex &index);
private:
    keys_t m_keys;
    Ui::MainWindow *ui;
    bool m_private_mode;
    udp_chat::user_desc_s m_reciever;
};

