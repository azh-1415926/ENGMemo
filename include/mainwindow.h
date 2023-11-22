#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class settingFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    settingFile* m_Info;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    ;

public slots:
    ;

private slots:
    void saveAttribute(const QString& key,const QString& value);

signals:
    ;

private:
    void initalWindow();
    void initalSetting();
};

void saveFile(const QString& path,const QString& data);

#endif // MAINWINDOW_H