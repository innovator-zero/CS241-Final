#include <QtCharts>
using namespace QtCharts;
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tab1
    //checkboxes' default value is checked
    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox_2->setCheckState(Qt::Checked);
    ui->checkBox_3->setCheckState(Qt::Checked);
    ui->checkBox_4->setCheckState(Qt::Checked);
    ui->checkBox_5->setCheckState(Qt::Checked);
    ui->checkBox_6->setCheckState(Qt::Checked);
    ui->checkBox_7->setCheckState(Qt::Checked);
    ui->checkBox_8->setCheckState(Qt::Checked);
    ui->checkBox_9->setCheckState(Qt::Checked);

    //set combobox
    for(int i=0;i<=80;i++){
        ui->comboBox->addItem(tr("%1").arg(i));
    }
    ui->comboBox_2->addItem("A");
    ui->comboBox_2->addItem("B");
    ui->comboBox_2->addItem("C");

    //tab2
    //set DateTimeEdit
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());

    //tab3
    //set button icon
    QIcon myicon;
    myicon.addFile(tr(":/images/1.jpg"));
    ui->pushButton_2->setIcon(myicon);

    //overall
    //set timer
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    //set file opener
    QAction *openAction = ui->menuFile->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    QAction *quitAction = ui->menuFile->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    menuBar()->addMenu(ui->menuFile);
    statusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//overall
void MainWindow::openFile()
{
    //load file
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"), "", "*.csv");
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QTextStream in(&file);

    //set header
    QStringList header;
    header<<"time"<<"lineID"<<"stationID"<<"deviceID"<<"status"<<"userID"<<"payType";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setRowCount(1);

    //read file
    QString line;
    line=in.readLine();//skip the header
    QStringList fields;
    int row = 0;

    while (!in.atEnd())
    {
        line = in.readLine();
        fields = line.split(',');
        for (int column = 0; column < fields.size(); column++){
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(fields[column]));
            ui->tableWidget->item(row,column)->setTextAlignment(Qt::AlignCenter);//test align in cneter
        }
        row++;
        ui->tableWidget->insertRow(row);
    }

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()-1);//delete unnecessary row
    ui->tableWidget->resizeColumnsToContents();//self-adapt items
    ui->tableWidget->resizeRowsToContents();

    //set time filter in trend
    QDateTime s=QDateTime::fromString(ui->tableWidget->item(0,0)->data(Qt::DisplayRole).toString(),"yyyy-MM-dd hh:mm:ss");
    QDateTime e(s);
    QTime morning=QTime::fromString("05:00:00","hh:mm:ss");
    s.setTime(morning);
    QTime night=QTime::fromString("23:59:59","hh:mm:ss");
    e.setTime(night);
    ui->dateTimeEdit->setDateTime(s);
    ui->dateTimeEdit_2->setDateTime(e);
    ui->dateTimeEdit->setDateTimeRange(s,e);
    ui->dateTimeEdit_2->setDateTimeRange(s,e);

    statusBar()->showMessage(tr("Loading %1 completed, %2rows").arg(fileName).arg(row), 10000);
}

void MainWindow::timerUpdate(void){
    ui->label_12->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

//tab1
void MainWindow::on_checkBox_stateChanged(int arg1)//time
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(0,false);
    else
        ui->tableWidget->setColumnHidden(0,true);
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)//lineID
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(1,false);
    else
        ui->tableWidget->setColumnHidden(1,true);
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)//stationID
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(2,false);
    else
        ui->tableWidget->setColumnHidden(2,true);
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)//deviceID
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(3,false);
    else
        ui->tableWidget->setColumnHidden(3,true);
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)//status
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(4,false);
    else
        ui->tableWidget->setColumnHidden(4,true);
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)//userID
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(5,false);
    else
        ui->tableWidget->setColumnHidden(5,true);
}

void MainWindow::on_checkBox_7_stateChanged(int arg1)//payType
{
    if(arg1==Qt::Checked)
        ui->tableWidget->setColumnHidden(6,false);
    else
        ui->tableWidget->setColumnHidden(6,true);
}

void MainWindow::on_checkBox_8_stateChanged(int arg1)//status:out
{
    if(ui->tableWidget->item(0,0)==0){return;}//avoid error in initialization
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,4)->data(Qt::DisplayRole).toString()=="0"){
            if(arg1==Qt::Checked){
                ui->tableWidget->setRowHidden(i,false);
            }
            else {
                ui->tableWidget->setRowHidden(i,true);
            }
        }
    }
}

void MainWindow::on_checkBox_9_stateChanged(int arg1)//status:in
{
    if(ui->tableWidget->item(0,0)==0){return;}//avoid error in initialization
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,4)->data(Qt::DisplayRole).toString()=="1"){
            if(arg1==Qt::Checked){
                ui->tableWidget->setRowHidden(i,false);
            }
            else {
                ui->tableWidget->setRowHidden(i,true);
            }
        }
    }
}

void MainWindow::on_pushButton_5_clicked()//filter:lineID
{
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,1)->data(Qt::DisplayRole).toString()==ui->comboBox_2->currentText()){
            ui->tableWidget->setRowHidden(i,false);
        }
        else {
            ui->tableWidget->setRowHidden(i,true);
        }
    }
}

void MainWindow::on_pushButton_4_clicked()//filter:stationID
{
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,2)->data(Qt::DisplayRole).toString()==ui->comboBox->currentText()){
            ui->tableWidget->setRowHidden(i,false);
        }
        else {
            ui->tableWidget->setRowHidden(i,true);
        }
    }
}

void MainWindow::on_pushButton_6_clicked()//diaplay all
{
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        ui->tableWidget->setRowHidden(i,false);
    }
    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox_2->setCheckState(Qt::Checked);
    ui->checkBox_3->setCheckState(Qt::Checked);
    ui->checkBox_4->setCheckState(Qt::Checked);
    ui->checkBox_5->setCheckState(Qt::Checked);
    ui->checkBox_6->setCheckState(Qt::Checked);
    ui->checkBox_7->setCheckState(Qt::Checked);
    ui->checkBox_8->setCheckState(Qt::Checked);
    ui->checkBox_9->setCheckState(Qt::Checked);
}

//tab2
void MainWindow::on_pushButton_3_clicked()
{
    if(ui->tableWidget->item(0,0)==0){
        QMessageBox::about(nullptr, "Wrong", "You must load file first!");
        return;
    }
    //avoid runtime error

    //get information from user
    int station=ui->spinBox_3->value();
    int timeinterval=(ui->horizontalScrollBar->value())*60;//seconds
    QDateTime starttime=ui->dateTimeEdit->dateTime();
    QDateTime endtime=ui->dateTimeEdit_2->dateTime();
    if(starttime>=endtime){
        QMessageBox::about(nullptr, "Wrong", "Starting time must be earlier than ending time!");
        return;
    }
    int timenum=int(endtime.toTime_t()-starttime.toTime_t())/timeinterval+1;//calculate the number of bars

    //set two groups
    QSplineSeries *series1=new QSplineSeries();
    QSplineSeries *series2=new QSplineSeries();
    series1->setName("in");
    series2->setName("out");
    int *in=new int[timenum];
    int *out=new int[timenum];
    for(int i=0;i<timenum;i++){
        in[i]=0;
        out[i]=0;
    }

    //get statistics
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,2)->data(Qt::DisplayRole).toInt()==station)//station filter
        {
            QDateTime tmp=QDateTime::fromString(ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toString(),"yyyy-MM-dd hh:mm:ss");
            if(tmp>=starttime){
                int j=int(tmp.toTime_t()-starttime.toTime_t())/timeinterval;
                if(ui->tableWidget->item(i,4)->data(Qt::DisplayRole).toInt()==1){
                    in[j]++;
                }
                else{
                    out[j]++;
                }
            }
        }
    }
    int M=0;
    for(int i=0;i<timenum;i++){
        series1->append(starttime.toMSecsSinceEpoch()+i*timeinterval*1000,in[i]);
        series2->append(starttime.toMSecsSinceEpoch()+i*timeinterval*1000,out[i]);
        if(in[i]>M)M=in[i];//look for the largest
        if(out[i]>M)M=out[i];
    }

    //draw graph
    QChart *chart = new QChart();
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->setTitle("Trend of Traffic Inflow and Outflow");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    //set axisX
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(timenum);
    axisX->setFormat("hh:mm");
    axisX->setTitleText("Time");
    axisX->setLabelsAngle(300);
    chart->addAxis(axisX, Qt::AlignBottom);
    series1->attachAxis(axisX);
    series2->attachAxis(axisX);

    //set axisY
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setRange(0,(M/10+2)*10);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    ui->widget->setChart(chart);
    ui->widget->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)//display time interval
{
    ui->label_5->setNum(position);
}

//tab3
void MainWindow::DFS(int nowPoint, int count)
{
    visited[nowPoint] = 1;
    path[count++] = nowPoint;
    if (nowPoint == end && count >= 1)//find a new route
    {
        pathNumber++;
        if(count<leastcount){//best route
            leastcount=count;
            for(int j=0;j<count;j++){
                bestpath[j]=path[j];
            }
        }
        ui->textEdit->append(tr("No.%1:").arg(pathNumber));
        for (int i=0;i<count-1; i++)
        {
            ui->textEdit->insertPlainText(tr("%1->").arg(path[i]));
        }
        ui->textEdit->insertPlainText(tr("%1\n").arg(path[count-1]));
    }
    else
    {
        for (int k=0; k<=80; k++)
        {
            if (map[nowPoint][k]==1 && visited[k] == 0)
                DFS(k, count);
        }
    }
    visited[nowPoint] = 0;
    count--;
    return;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->setPlainText("");//clear textedit
    //get information from user
    start=ui->spinBox->value();
    end=ui->spinBox_2->value();

    //dfs
    pathNumber=0;
    leastcount=81;
    DFS(start,0);

    //show best route
    ui->textEdit->append("Best:");
    for (int i = 0; i < leastcount-1; i++)
    {
        ui->textEdit->insertPlainText(tr("%1->").arg(bestpath[i]));
    }
    ui->textEdit->insertPlainText(tr("%1\n").arg(bestpath[leastcount-1]));
    statusBar()->showMessage(tr("Routes planing completed"), 10000);
}

void MainWindow::on_pushButton_2_clicked()//switch entering and exiting
{
    int a=ui->spinBox->value();
    int b=ui->spinBox_2->value();
    ui->spinBox->setValue(b);
    ui->spinBox_2->setValue(a);
}
