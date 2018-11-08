#ifndef SERIAL_TEXTIOF_0_1_H
#define SERIAL_TEXTIOF_0_1_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QApplication>




void text_Sending();
char* text_Reading();
int parsingCommand(char*);

void processing_checkpoint();


#endif // SERIAL_TEXTIOF_0_1_H
