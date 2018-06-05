#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPixmap>
#include <QImageReader>
#include <QImage>
#include <QFile>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>

#include "fileio.h"
#include "map.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitToolBox();
    void InitMap();
    Map* mapHandle;
    QPixmap* imageHandle;
    //QPixmap imageHandle;
    void createStatusBar();
    QString currentWorkingPath;
    QString cfgFilePath;
    QString resourcePath;
    QString xbeeAddrPath;
    bool inMapFlag;
    bool leftDown;
    bool rightDown;
    int mouseX;
    int mouseY;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void draw(QPainter *painter);

protected:
    //void resizeEvent(QResizeEvent *event) override;
signals:
    void paintRequest();
public slots:
    void updatePaint();
private slots:
    void on_plusButton_clicked();

    void on_minusButton_clicked();

    void on_downloadButton_clicked();

    void on_returnButton_clicked();

    void on_currentLevelSlider_valueChanged(int value);

    void on_manageLevelSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
