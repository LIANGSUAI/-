#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_YourMainWindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "huffmantree.h"
#include "HuffmanCompressor.h"
#include <QTextCodec>

class YourMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    YourMainWindow(QWidget *parent = nullptr);
    ~YourMainWindow();
    void displayFrequencyInfo(const QMap<QChar, int>& frequencyMap);
    void calculateCharacterFrequency(const QString& fileName, QMap<QChar, int>& frequencyMap);
    QMap<QChar, int> get_characterFrequencyMap();

private slots:
    void on_selectFileButton_clicked();
    void on_compressButton_clicked();
    void on_decompressButton_clicked();
    void on_showEncodingTableButton_clicked();

private:
    Ui::YourMainWindowClass ui;
    HuffmanCompressor huffmanCompressor_raw;
    QString selectedFilePath;                // 用于保存用户选择的文件路径
    QMap<QChar, int> characterFrequencyMap;  // 用于保存文件字符频率表
};
