#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>

#include <QLayout>
#include <QLabel>
#include <QFont>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>


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

    QWidget *main_Widget;
    QGridLayout *layout;
    QPushButton *push_Button;
    QLabel *receiving_Message;
    QSerialPort *port;
    QSerialPort *port_sen;
    QLineEdit *line_Edit;
    QLabel *received_Data;
    int32_t init_angle;
    int32_t final_angle;
    int32_t resoluton;
    int32_t iter_cnt;
    int32_t cnt;
    bool    get_snitch_flag;
    bool    send_snitch_flag;

public slots:
    void text_Sending();
    void text_Reading();
    int parsingCommand(char*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
