#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //ui->setupUi(this);
    get_snitch_flag = false;
    send_snitch_flag = false;

//    main_Widget = new QWidget(this);
//    this->setCentralWidget(main_Widget);

//    layout = new QGridLayout();
//    main_Widget->setLayout(layout);

//    push_Button = new QPushButton("Send message : ");
//    layout->addWidget(push_Button);

//    line_Edit = new QLineEdit();
//    line_Edit->setFixedSize(100, 20);
//    layout->addWidget(line_Edit);

//    receiving_Message = new QLabel("Received message : ");
//    receiving_Message->setStyleSheet("QLabel{ color : blue}");
//    QFont font("Arial", 12, QFont::Bold);
//    receiving_Message->setFont(font);
//    layout->addWidget(receiving_Message);

//    received_Data = new QLabel(".. Waiting renewal ..");
//    layout->addWidget(received_Data);

    port = new QSerialPort();
    port->setPortName("COM4");
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    if(!port->open(QIODevice::ReadWrite))
    {
        qDebug() << "\n Serial port open error \n";
    }

    port_sen = new QSerialPort();
    port_sen->setPortName("COM44");
    port_sen->setBaudRate(QSerialPort::Baud9600);
    port_sen->setDataBits(QSerialPort::Data8);
    port_sen->setParity(QSerialPort::NoParity);
    port_sen->setStopBits(QSerialPort::OneStop);
    port_sen->setFlowControl(QSerialPort::NoFlowControl);

    if(!port_sen->open(QIODevice::ReadWrite))
    {
        qDebug() << "\n Serial port open error \n";
    }

//    QObject::connect(push_Button, SIGNAL(clicked(bool)), this, SLOT(text_Sending()));
    QObject::connect(port, SIGNAL(readyRead()), this, SLOT(text_Reading()));
}

MainWindow::~MainWindow()
{
   port->close();
   port_sen->close();
}

void MainWindow::text_Sending()
{
    QByteArray send_Data;
    send_Data = line_Edit->text().toUtf8().left(1);
    port->write(send_Data.data());
}

void MainWindow::text_Reading()
{
//    QByteArray read_Data;
//    read_Data = port->read(2);
//    read_Data = port->readAll();
//    char* byte_datas = read_Data.data();
    char* parsed_datas;
    char* byte_datas;
    int num_index =0;

    num_index = parsingCommand(parsed_datas);

//    received_Data->setText(parsed_datas);
//    received_Data->show();

    QFile *file = new QFile;
    QString filename = "sample.txt";
    file->setFileName(QApplication::applicationDirPath() + "/" + filename);
    if(!file->exists()){
        qDebug() << "NO exist"<<filename;
    }else{
        qDebug() << filename<<" exist";
    }

//    if(num_index == 9)
//    {
//      init_angle  = (int32_t)(parsed_datas[0]<<8 + parsed_datas[1]);
//      final_angle = (int32_t)(parsed_datas[2]<<8 + parsed_datas[3]);
//      resoluton   = (int32_t)(parsed_datas[4]<<8 + parsed_datas[5]);
//      iter_cnt    = (int32_t)parsed_datas[6];
      *byte_datas = (parsed_datas[6] + '0');
//    }
//    else if(num_index == 3)
//    {

//    }
    QString mesg= byte_datas;

    if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(file);
        out<<mesg;
        file->close();
    }
}


int MainWindow::parsingCommand(char* result)
{
  int i =0;
  QByteArray temp_data;
  temp_data = port->read(1);
  char* temp = temp_data.data();

  if(*temp == '#')
  {
    qDebug() << "#";
    i++;
    while(*temp!='@')
    {
      qDebug() << temp_data;

      temp_data = port->read(1);
      temp = temp_data.data();
      result[i-1] = *temp;
      i++;
    }
     qDebug() << "END";
  }
  return i;
}
