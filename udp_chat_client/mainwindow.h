#pragma once

#include <QMainWindow>
#include <QString>
#include "client.hpp"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void reset();
signals:
    void send_msg(const QString &msg);
public slots:
    void recieve_msg(const QString &msg);
    void append_user(const QString &nickname);
    void remove_user(const QString &nickname);
private slots:
    void button_clicked();
private:
    Ui::MainWindow *ui;
};

