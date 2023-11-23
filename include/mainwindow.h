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
    QJsonObject getWordsByDate(const QString& timeOfYMD);
    void updateWords();
};

void saveFile(const QString& path,const QString& data);
void saveWords(const QString& path,const QJsonObject& words);

#endif // MAINWINDOW_H