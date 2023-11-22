#include "mainwindow.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "settingFile.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_Info(new settingFile)
{
    initalSetting();
    initalWindow();
}

MainWindow::~MainWindow()
{
    delete m_Info;
}

/* 保存属性的值，保存到 info.json 文件 */
void MainWindow::saveAttribute(const QString &key, const QString &value)
{
    /* 获取当前时间 */
    QDateTime time=QDateTime::currentDateTime();
    /* 转化为 "yyyy-MM-dd" 格式的字符串，效果为 2023-01-01 */
    QString timeOfYMD=time.toString("yyyy-MM-dd");
    /* 转化为 "hh:mm:ss" 格式的字符串，效果为 06:30 */
    QString timeOfHM=time.toString("hh:mm");
    /* 文件名为日期名+.json，并把该日期添加到 info.json */
    m_Info->add(timeOfYMD,"data/"+timeOfYMD+".json");
    m_Info->save("info.json");
    /* 试图打开 data/日期.json */
    QDir dir(QDir::currentPath());
    if(!dir.exists("data"))
        dir.mkdir("data");
    settingFile file("data/"+timeOfYMD+".json");
    /* 试图获取 json 文件中的 timeOfHM 关键字对应的单词列表 */
    auto words=file.value(timeOfHM);
    if(words.isUndefined())
    {
        QJsonObject obj;
        obj.insert(key,value);
        file.add(timeOfHM,obj);
    }
    else
    {
        auto obj=words.toObject();
        obj.insert(key,value);
        file.add(timeOfHM,obj);
    }
    file.save("data/"+timeOfYMD+".json");

}

void MainWindow::initalSetting()
{
    QFile file("info.json");
    if(!(file.exists()))
        m_Info->load(":/json/info.json");
    else
        m_Info->load("info.json");
}

void MainWindow::initalWindow()
{
    QWidget* widget=new QWidget;
    this->setCentralWidget(widget);
    QHBoxLayout* layout=new QHBoxLayout(widget);
    QLineEdit* inputOfWord=new QLineEdit;
    QLineEdit* inputOfExplain=new QLineEdit;
    QPushButton* buttonOfSubmit=new QPushButton("submit");
    layout->addWidget(inputOfWord);
    layout->addWidget(inputOfExplain);
    layout->addWidget(buttonOfSubmit);
    connect(buttonOfSubmit,&QPushButton::clicked,this,[=]()
    {
        const QString& word=inputOfWord->text();
        const QString& explain=inputOfExplain->text();
        if(!(word.isEmpty()||explain.isEmpty()))
            saveAttribute(word,explain);
        inputOfWord->clear();
        inputOfExplain->clear();
    });
}

void saveFile(const QString &path,const QString& data)
{
    QFile file(path);
    /* 保存到 path 文件中（只写、追加保存） */
    if(file.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        /* stream 处理文件，设置编码为 utf-8 */
        QTextStream stream(&file);
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            stream.setCodec("utf-8");
        #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
            stream.setEncoding(QStringConverter::Utf8);
        #endif
        /* 导出并关闭文件 */
        stream<<data;
        file.close();
    }
    else
        qDebug()<<"error","save \""+path+"\" is failed!";
}