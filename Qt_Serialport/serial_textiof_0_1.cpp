#include "serial_textiof_0_1.h"

QSerialPort *port;
QSerialPort *port_sen;

int32_t init_angle;
int32_t final_angle;
int32_t resoluton;
int32_t iter_cnt;
int32_t cnt;
bool    get_snitch_flag;
bool    send_snitch_flag;

void text_Sending()
{

}

char* text_Reading()
{
  //    QByteArray read_Data;
  //    read_Data = port->read(2);
  //    read_Data = port->readAll();
  //    char* byte_datas = read_Data.data();
      char* parsed_datas = NULL;
      char* byte_datas = NULL;
      int num_index =0;

      num_index = parsingCommand(parsed_datas);
      QString mesg= parsed_datas;
      qDebug() << mesg;
  //    received_Data->setText(parsed_datas);
  //    received_Data->show();


  //    if(num_index == 9)
  //    {
  //      init_angle  = (int32_t)(parsed_datas[0]<<8 + parsed_datas[1]);
  //      final_angle = (int32_t)(parsed_datas[2]<<8 + parsed_datas[3]);
  //      resoluton   = (int32_t)(parsed_datas[4]<<8 + parsed_datas[5]);
  //      iter_cnt    = (int32_t)parsed_datas[6];
//        *byte_datas = (parsed_datas[6] + '0');
  //    }
  //    else if(num_index == 3)
  //    {

  //    }

  return byte_datas;

}

int parsingCommand(char* result)
{
  int i =0;
  QByteArray temp_data;
  if(port->waitForReadyRead()) temp_data = port->read(1);
  char* temp = temp_data.data();

  if(*temp == '#')
  {
    qDebug() << "#";


    if(port->waitForReadyRead()) temp_data = port->read(1);
    temp = temp_data.data();
    i++;

    while(*temp != '@')
    {
      qDebug() << temp_data;
      result[i-1] = *temp;

      if(port->waitForReadyRead()) temp_data = port->read(1);
      temp = temp_data.data();
      i++;
    }
     qDebug() << "END";
  }
  return i;
}

void processing_checkpoint()
{
  get_snitch_flag = false;
  send_snitch_flag = false;

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

  QFile *file = new QFile;
  QString filename = "sample.txt";
  file->setFileName(QApplication::applicationDirPath() + "/" + filename);
  if(!file->exists()){
      qDebug() << "NO exist"<<filename;
  }else{
      qDebug() << filename<<" exist";
  }


  if(!port_sen->open(QIODevice::ReadWrite))
  {
    qDebug() << "\n Serial port open error \n";
  }

  while(1)
  {
    if(port->waitForReadyRead())
    {
      qDebug() << "\n get in ";
       QString mesg = text_Reading();
//       if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
//           QTextStream out(file);
//           out<<mesg;
//           file->close();
//       }
    }

  }
}
