#ifndef DISPALYTABLE_H
#define DISPALYTABLE_H

#include <QtWidgets/QMainWindow>
#include "ui_displaytable.h"
#include <QFile>
#include <QTextStream>
#include<QFileDialog>
#include<QTableWidgetItem>
#include <iostream>
#include <vector>
using namespace std;
class displayTable : public QMainWindow
{
    Q_OBJECT

public:
    displayTable(QWidget *parent = Q_NULLPTR);
public slots:
    void on_openButton_clicked();
    void on_selectButton_clicked();
private:
    Ui::displayTableClass ui;
    vector<QString>linesStr;
};

#endif // DISPALYTABLE_H
