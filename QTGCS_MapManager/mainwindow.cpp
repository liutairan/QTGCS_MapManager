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
    ui->currentLevelLabel->setText("C. L.\n  " + QString::number(ui->currentLevelSlider->value(),10));
    ui->manageLevelLabel->setText("M. L.\n  " + QString::number(ui->manageLevelSlider->value(),10));
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
    QPixmap tempMap = mapHandle->retImage.scaled(QSize(640,640));

    if (tempMap.isNull())
    {
        QString blackPath = resourcePath + "/res/black.jpg";
        imageHandle->load(blackPath);
    }
    else
    {
        imageHandle = &tempMap;
    }

    QPainter painter(imageHandle);

    draw(&painter);
    ui->mapLabel->setPixmap(*imageHandle);
    this->repaint();
}

void MainWindow::draw(QPainter *painter)
{
    QPen Red((QColor::fromRgb(255,0,0)),10);
    QPen YellowDot((QColor::fromRgb(255,255,0)),10);
    QPen GreenLine((QColor::fromRgb(0,255,0)),1);
    QPen BlueLine((QColor::fromRgb(0,0,255)),1);
    painter->setPen(GreenLine);
    QVector<QPoint> ppVector;
    uint divider = 8;
    uint spacer = 640/divider;
    for (uint i = 0; i<=divider; i++)
    {
        ppVector.append(QPoint(i*spacer, 0));
        ppVector.append(QPoint(i*spacer, 640));
        ppVector.append(QPoint(0, i*spacer));
        ppVector.append(QPoint(640, i*spacer));
    }
    painter->drawLines(ppVector);
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
    if (mapHandle->_zoomlevel <= 21)
    {
        mapHandle->zoom(1);
        int manageLevelValue = ui->manageLevelSlider->value() + 1;
        ui->manageLevelSlider->setValue(manageLevelValue);
        emit paintRequest();
    }

}

void MainWindow::on_minusButton_clicked()
{
    if (mapHandle->_zoomlevel >= 9)
    {
        mapHandle->zoom(-1);
        int manageLevelValue = ui->manageLevelSlider->value() - 1;
        ui->manageLevelSlider->setValue(manageLevelValue);
        emit paintRequest();
    }
}

void MainWindow::on_downloadButton_clicked()
{

}

void MainWindow::on_returnButton_clicked()
{

}

void MainWindow::on_currentLevelSlider_valueChanged(int value)
{
    int currentViewLevel = mapHandle->_viewlevel;
    ui->currentLevelLabel->setText("C. L.\n  "+QString::number(value,10));
    if (value - ui->manageLevelSlider->value() < -3)
    {
        ui->manageLevelSlider->setValue(value+3);
    }
    if (value - ui->manageLevelSlider->value() > 0)
    {
        ui->manageLevelSlider->setValue(value);
    }
    if ((value >=9) && (value <=21))
    {
        mapHandle->viewZoom(value-currentViewLevel);
    }
    emit paintRequest();
}

void MainWindow::on_manageLevelSlider_valueChanged(int value)
{
    int currentManageLevel = mapHandle->_zoomlevel;
    ui->manageLevelLabel->setText("M. L.\n  "+QString::number(value,10));
    if (value - ui->currentLevelSlider->value() < 0)
    {
        ui->currentLevelSlider->setValue(value);
    }
    if (value - ui->currentLevelSlider->value() > 3)
    {
        ui->currentLevelSlider->setValue(value-3);
    }
    if ((value >=9) && (value <=21))
    {
        mapHandle->zoom(value-currentManageLevel);
    }
    emit paintRequest();
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
