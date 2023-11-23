#include "mainwindow.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "settingFile.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_Info(new settingFile)
{
    initalSetting();
    initalWindow();
    updateWords();
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

QJsonObject MainWindow::getWordsByDate(const QString &timeOfYMD)
{
    QJsonObject data;
    settingFile file("data/"+timeOfYMD+".json");
    if(!(file.isLoad()))
        return data;
    const QJsonObject& json=file.toJson();
    for(const auto& i : json.keys())
    {
        const auto& wordsOfMinute=json.value(i).toObject();
        for(const auto& j : wordsOfMinute.keys())
        {
            QJsonObject word;
            QJsonArray arrayOfTime;
            arrayOfTime.push_back(timeOfYMD+" "+i);
            word.insert("time",arrayOfTime);
            word.insert("explain",wordsOfMinute.value(j).toString());
            data.insert(j,word);
        }
    }
    return data;
}

void MainWindow::updateWords()
{
    QRegularExpression reg("\\d{4}-\\d{2}-\\d{2}");
    QRegularExpressionValidator v(reg,0);
    int pos=0;
    QValidator::State result;
    const auto& fileOfWords=m_Info->value("FileOfWords").toString();
    for(auto& i : m_Info->keys())
    {
        pos=0;
        result=v.validate(i,pos);
        if(result==QValidator::State::Acceptable)
        {
            auto json=getWordsByDate(i);
            saveWords(fileOfWords,json);
        }
    }
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

void saveWords(const QString &path, const QJsonObject &words)
{
    /* 试图打开 path 路径下的 json 文件 */
    settingFile file(path);
    /* 若打开失败，则直接以 words 作为配置文件，并保存 */
    if(!(file.isLoad()))
    {
        file.load(words);
        file.save(path);
        return;
    }
    /* 遍历 words，将所有单词追加到 file 中的 json 文件中 */
    for(const auto& word : words.keys())
    {
        /* info 为单词 i 对应的值 */
        auto info=file.value(word);
        /* infoOfWord 对应单词信息 */
        QJsonObject infoOfWord;
        const auto& obj=words.value(word).toObject();
        /* 若 info 不存在，则将 words 中的变量赋给它  */
        if(info.isUndefined())
            infoOfWord=obj;
        /* 若 info 存在，则赋值给对应变量  */
        else
        {
            infoOfWord=info.toObject();
            auto arrayOfTime=infoOfWord.value("time").toArray();
            /* 对于 time */
            for(const auto& time : obj.value("time").toArray())
            {
                if(!(arrayOfTime.contains(time)))
                    arrayOfTime.push_back(time);
            }
                
            /* 更新 infoOfWord 中的 time  */
            infoOfWord.insert("time",arrayOfTime);
        }
        file.add(word,infoOfWord);
    }
    file.save(path);
}