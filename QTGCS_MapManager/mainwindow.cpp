#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    removeToolBar(ui->mainToolBar);
    createStatusBar();
    currentWorkingPath = QCoreApplication::applicationDirPath();
    cfgFilePath = currentWorkingPath + "/config.json";

    QByteArray val;
    val = readJsonFile(cfgFilePath);
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(val);
    QJsonObject obj = doc.object();
    resourcePath = obj["Resource"].toString();

    InitToolBox();
    InitMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::readJsonFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        f.close();
        return QString().toUtf8();
    } else {
        QTextStream in(&f);
        QByteArray retValue = in.readAll().toUtf8();
        f.close();
        return retValue;
    }
}

void MainWindow::InitToolBox()
{
    ui->plusButton->move(640, 10);
    ui->minusButton->move(640, 50);
    ui->downloadButton->move(640, 90);
    ui->returnButton->move(640, 130);
}

void MainWindow::InitMap()
{
    ui->mapLabel->setGeometry(0,0,640,640);
    QString blackPath = resourcePath + "/res/black.jpg";
    QImageReader r(blackPath);
    QImage i = r.read();
    if (!i.isNull())
    {
        imageHandle = QPixmap::fromImage(i);
    }
    //QPainter painter(imageHandle);

    //draw(&painter);
    ui->mapLabel->setPixmap(imageHandle);
    this->repaint();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    QSize size = this->size();
//    statusBar()->showMessage("Size"+QString::number(size.width(), 10) + QString::number(size.height(),10));
//}

void MainWindow::on_plusButton_clicked()
{

}

void MainWindow::on_minusButton_clicked()
{

}

void MainWindow::on_downloadButton_clicked()
{

}

void MainWindow::on_returnButton_clicked()
{

}
