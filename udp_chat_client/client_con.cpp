#include "client_con.h"
#include "ui_client_con.h"

client_con::client_con(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client_con)
{
    ui->setupUi(this);
}

client_con::~client_con()
{
    delete ui;
}

void client_con::on_pushButton_connect_clicked()
{
    emit(create_client());
}

QString client_con::get_nickname()
{
    QString nickname;
    nickname=ui->nickname->text();
    return nickname;
}
quint16 client_con::get_port()
{
    qint16 port;
    bool ok;
    port=ui->server_port->text().toUInt(&ok,10);
    return port;
}
QHostAddress client_con::get_addr()
{
    QHostAddress addr;
    addr=ui->server_addr->text();
//    addr=QHostAddress::LocalHost;
    return addr;
}
