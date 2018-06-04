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

void MainWindow::InitToolBox()
{
    ui->plusButton->move(640, 10);
    ui->minusButton->move(640, 50);
    ui->downloadButton->move(640, 90);
    ui->returnButton->move(640, 130);
    ui->currentLevelSlider->move(645,200);
    ui->manageLevelSlider->move(645,390);
    ui->currentLevelLabel->move(640,170);
    ui->manageLevelLabel->move(640,360);
    ui->currentLevelLabel->setText("C. L.\n  3");
    ui->manageLevelLabel->setText("M. L.\n  3");
}

void MainWindow::InitMap()
{
    ui->mapLabel->setGeometry(0,0,640,640);
    mapHandle = new Map(30.4081580, -91.1795330, 19, 640, 640);
    connect(this, SIGNAL(paintRequest()), this, SLOT(updatePaint()));

    //ui->zoomLevelLabel->setText("Zoom Level: " + QString::number(mapHandle->_zoomlevel, 10));
    emit paintRequest();

//    QString blackPath = resourcePath + "/res/black.jpg";
//    QImageReader r(blackPath);
//    QImage i = r.read();
//    if (!i.isNull())
//    {
//        imageHandle = QPixmap::fromImage(i);
//    }
//    //QPainter painter(imageHandle);

//    //draw(&painter);
//    ui->mapLabel->setPixmap(imageHandle);
//    this->repaint();
}

void MainWindow::updatePaint()
{
    QPixmap tempMap = mapHandle->retImage;

    if (tempMap.isNull())
    {
        //QString currentWorkingPath = QDir::currentPath();
        QString blackPath = resourcePath + "/res/black.jpg";
        imageHandle->load(blackPath);
    }
    else
    {
        imageHandle = &tempMap;
    }
    //QPainter painter(imageHandle);

    //draw(&painter);
    ui->mapLabel->setPixmap(*imageHandle);
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

void MainWindow::on_currentLevelSlider_valueChanged(int value)
{
    ui->currentLevelLabel->setText("C. L.\n  "+QString::number(value,10));
}

void MainWindow::on_manageLevelSlider_valueChanged(int value)
{
    ui->manageLevelLabel->setText("M. L.\n  "+QString::number(value,10));
}

// mouse events
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        leftDown = true;
        mouseX = event->x();
        mouseY = event->y();
        //qDebug() << "Left Down "<<event->x()<<"  "<<event->y();
        //qDebug() << ui->maplabel->rect().contains(event->pos());
    }
    if(event->button() == Qt::RightButton)
    {
        rightDown = true;
        //qDebug() << "Right Down "<<event->x()<<"  "<<event->y() << deHandle->get_serialMode();
    }
}

void MainWindow::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button() == Qt::LeftButton)
    {
        leftDown = false;
        //qDebug() << "Left Up";
    }
    if(event->button() == Qt::RightButton)
    {
        rightDown = false;
        //qDebug() << "Right Up";
        if (ui->mapLabel->rect().contains(event->pos()))
        {
            PixelPosition tempPoint;
            tempPoint.x = event->pos().x();
            tempPoint.y = event->pos().y();
            GPSCoordinate tempCo = mapHandle->PostoGPS(tempPoint);
        }
    }
}

void MainWindow::mouseMoveEvent ( QMouseEvent * event )
{
    //Show x and y coordinate values of mouse cursor here
    //this->setText("X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y()));
    if ((ui->mapLabel->rect().contains((event->pos()))) && leftDown == true)
    {
        int dx = event->x() - mouseX;
        int dy = event->y() - mouseY;
        mapHandle->move(dx,dy);
        emit paintRequest();
    }
}
