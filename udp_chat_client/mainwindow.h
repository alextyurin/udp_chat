#pragma once

#include <QMainWindow>
#include <QString>
#include "client.hpp"

namespace Ui {
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
private slots:
    void button_clicked();
    void recieve_msg(const QString &msg);
private:
    Ui::MainWindow *ui;
};
