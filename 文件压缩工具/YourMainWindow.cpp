#include "YourMainWindow.h"
#include <QDebug>
#include <QVBoxLayout>


YourMainWindow::YourMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

QMap<QChar, int> YourMainWindow::get_characterFrequencyMap() {
    return characterFrequencyMap;
};

void YourMainWindow::on_selectFileButton_clicked() {
    selectedFilePath = QFileDialog::getOpenFileName(this, u8"选择要压缩的文本文件", "", "All Files (*)");
    if ( !selectedFilePath.isEmpty()) {
         QFile file(selectedFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");  // 设置文件的编码方式为UTF-8
            QTextCodec* codec = QTextCodec::codecForName("UTF-8");
            QString content = codec->toUnicode(in.readAll().toUtf8());
            file.close();
            calculateCharacterFrequency(content, characterFrequencyMap);
            displayFrequencyInfo(characterFrequencyMap);
            ui.lineEditFilePath->setText(selectedFilePath); 
            ui.textEdit1->setPlainText(content);
        }
        else {
             QMessageBox::warning(this, "Error", "Failed to open the file.");
        }
    }
}

void YourMainWindow::displayFrequencyInfo(const QMap<QChar, int>& frequencyMap) {
    // 清空文本框内容
    ui.dialog->clear();
    ui.dialog->setFont(QFont());
    // 使用 QTextCursor 插入字符频率信息到 QTextEdit
    QTextCursor cursor(ui.dialog->document());
    cursor.movePosition(QTextCursor::Start);

    // 插入标题
    cursor.insertText("Character Frequency Information:\n");

    // 遍历频率映射并插入信息
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        // 插入文本并设置为红色
        cursor.insertText(QString("Character: %1, Frequency: %2\n").arg(it.key()).arg(it.value()), QTextCharFormat());

    }

    // 强制处理事件循环，确保界面更新
    QCoreApplication::processEvents();
}

void YourMainWindow::calculateCharacterFrequency(const QString& text, QMap<QChar, int>& frequencyMap) {
    for (const QChar& ch : text) {
        // 使用 Unicode 编码方式，确保能够正确处理中文字符
        frequencyMap[ch]++;
    }
}

void YourMainWindow::on_compressButton_clicked() {
    if (!selectedFilePath.isEmpty()) {
        // 使用频率表创建 HuffmanCompressor 对象
        HuffmanCompressor huffmanCompressor(characterFrequencyMap);
        
        // 获取输出文件路径
        QString outputPath = QFileDialog::getSaveFileName(this, u8"选择压缩后文件存储位置", "", "Compressed Files (*.bin)");

        if (!outputPath.isEmpty()) {
            ui.dialog->setText(u8"压缩中");
            // 执行压缩操作
           QString compressedFilePath = huffmanCompressor.compressFile(selectedFilePath, outputPath);

           QString encodingInfo = "Encoding Table:\n" + huffmanCompressor.getEncodingTable() + "\n\n";
           QString huffmanTreeInfo = "Huffman Tree:\n" + huffmanCompressor.getHuffmanTree() + "\n";
           
            // 显示成功信息或处理结果
           huffmanCompressor_raw = huffmanCompressor;
           ui.dialog->setText(u8"文件压缩成功！");
            QMessageBox::information(this, u8"压缩文件", u8"压缩文件成功！\n压缩后文件位于：" + compressedFilePath);
            ui.lineEditOutput->setText(outputPath);
        }
        else {
            QMessageBox::warning(this, u8"压缩文件", u8"压缩取消或输出文件未被选择");
        }
    }
    else {
        QMessageBox::warning(this, u8"错误", u8"打开文件失败");
    }
}

void YourMainWindow::on_decompressButton_clicked() {
    if (!selectedFilePath.isEmpty()) {
        ui.dialog->clear();
        QString compressedFilePath = selectedFilePath;
        QString outputPath = QFileDialog::getSaveFileName(this, u8"保存解压文件", "", "Text Files (*.txt)");
        if (!compressedFilePath.isEmpty()) {
            // 创建 HuffmanCompressor 对象
            HuffmanCompressor huffmanCompressor;
            // 执行解压缩操作
            ui.dialog->setText(u8"文件解压中...\n");
            huffmanCompressor_raw = huffmanCompressor;
            huffmanCompressor_raw.decompressFile(compressedFilePath, outputPath);
            if (!outputPath.isEmpty()) {
                // 显示成功信息或处理结果
                QMessageBox::information(this, u8"解压缩文件", u8"文件解压缩成功！\n解压后文件位于：" + outputPath);
                ui.lineEditOutput->setText(outputPath);
                ui.dialog->setText("decompressed successfully!\n");
            }
            else {
                QMessageBox::warning(this, u8"解压缩文件", u8"解压失败或输出文件未被选择");
            }
        }
        else {
            QMessageBox::warning(this, u8"错误", u8"未打开文件");
        }
    }
}

void YourMainWindow::on_showEncodingTableButton_clicked() {
    // 创建一个新的窗口
    QDialog* encodingTableDialog = new QDialog(this);
    encodingTableDialog->setWindowTitle(u8"编码表");

    // 创建一个文本框用于显示编码表
    QTextEdit* textEdit = new QTextEdit(encodingTableDialog);
    qDebug() << "Encoding Table:" << huffmanCompressor_raw.getEncodingTable();
    textEdit->setPlainText(huffmanCompressor_raw.getEncodingTable()); // getEncodingTable() 返回编码表的字符串
    textEdit->setReadOnly(true);

    // 布局管理
    QVBoxLayout* layout = new QVBoxLayout(encodingTableDialog);
    layout->addWidget(textEdit);
    encodingTableDialog->setLayout(layout);

    // 显示窗口
    encodingTableDialog->exec();
}


YourMainWindow::~YourMainWindow()
{}
