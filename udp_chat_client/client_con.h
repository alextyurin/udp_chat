#ifndef CLIENT_CON_H
#define CLIENT_CON_H

#include <QWidget>
#include<QHostAddress>

namespace Ui {
class client_con;
}

class client_con : public QWidget
{
    Q_OBJECT

public:
    explicit client_con(QWidget *parent = 0);
    ~client_con();
    QHostAddress get_addr();
    quint16 get_port();
    QString get_nickname();
signals:
    void create_client();
private slots:
    void on_pushButton_connect_clicked();


private:
    Ui::client_con *ui;
};

#endif // CLIENT_CON_H
