#include "mainwindow.h"
#include <QApplication>

bool CheckCFGFile(QString filePath)
{
    if (!QFileInfo(filePath).exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Configuration not exist. Please configure first.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    else
    {
        return true;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString currentWorkingPath = QCoreApplication::applicationDirPath();
    QString cfgFilePath = currentWorkingPath + "/config.json";

    if (CheckCFGFile(cfgFilePath))
    {
        MainWindow w;
        w.setFixedSize(640+32,667);
        w.show();
        w.setFocus();
        return a.exec();
    }
    else
    {
        return 0;
    }
//    MainWindow w;
//    w.setFixedSize(640+32,667);
//    w.show();

//    return a.exec();
}
