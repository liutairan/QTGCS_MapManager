#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPixmap>
#include <QImageReader>
#include <QImage>
#include <QFile>
#include <QJsonDocument>

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
    QPixmap imageHandle;
    void createStatusBar();
    QString currentWorkingPath;
    QString cfgFilePath;
    QString resourcePath;
    QString xbeeAddrPath;
    QByteArray readJsonFile(const QString &filename);
protected:
    //void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_plusButton_clicked();

    void on_minusButton_clicked();

    void on_downloadButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
