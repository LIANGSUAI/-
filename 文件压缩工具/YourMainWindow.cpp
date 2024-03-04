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
    selectedFilePath = QFileDialog::getOpenFileName(this, u8"ѡ��Ҫѹ�����ı��ļ�", "", "All Files (*)");
    if ( !selectedFilePath.isEmpty()) {
         QFile file(selectedFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");  // �����ļ��ı��뷽ʽΪUTF-8
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
    // ����ı�������
    ui.dialog->clear();
    ui.dialog->setFont(QFont());
    // ʹ�� QTextCursor �����ַ�Ƶ����Ϣ�� QTextEdit
    QTextCursor cursor(ui.dialog->document());
    cursor.movePosition(QTextCursor::Start);

    // �������
    cursor.insertText("Character Frequency Information:\n");

    // ����Ƶ��ӳ�䲢������Ϣ
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        // �����ı�������Ϊ��ɫ
        cursor.insertText(QString("Character: %1, Frequency: %2\n").arg(it.key()).arg(it.value()), QTextCharFormat());

    }

    // ǿ�ƴ����¼�ѭ����ȷ���������
    QCoreApplication::processEvents();
}

void YourMainWindow::calculateCharacterFrequency(const QString& text, QMap<QChar, int>& frequencyMap) {
    for (const QChar& ch : text) {
        // ʹ�� Unicode ���뷽ʽ��ȷ���ܹ���ȷ���������ַ�
        frequencyMap[ch]++;
    }
}

void YourMainWindow::on_compressButton_clicked() {
    if (!selectedFilePath.isEmpty()) {
        // ʹ��Ƶ�ʱ��� HuffmanCompressor ����
        HuffmanCompressor huffmanCompressor(characterFrequencyMap);
        
        // ��ȡ����ļ�·��
        QString outputPath = QFileDialog::getSaveFileName(this, u8"ѡ��ѹ�����ļ��洢λ��", "", "Compressed Files (*.bin)");

        if (!outputPath.isEmpty()) {
            ui.dialog->setText(u8"ѹ����");
            // ִ��ѹ������
           QString compressedFilePath = huffmanCompressor.compressFile(selectedFilePath, outputPath);

           QString encodingInfo = "Encoding Table:\n" + huffmanCompressor.getEncodingTable() + "\n\n";
           QString huffmanTreeInfo = "Huffman Tree:\n" + huffmanCompressor.getHuffmanTree() + "\n";
           
            // ��ʾ�ɹ���Ϣ������
           huffmanCompressor_raw = huffmanCompressor;
           ui.dialog->setText(u8"�ļ�ѹ���ɹ���");
            QMessageBox::information(this, u8"ѹ���ļ�", u8"ѹ���ļ��ɹ���\nѹ�����ļ�λ�ڣ�" + compressedFilePath);
            ui.lineEditOutput->setText(outputPath);
        }
        else {
            QMessageBox::warning(this, u8"ѹ���ļ�", u8"ѹ��ȡ��������ļ�δ��ѡ��");
        }
    }
    else {
        QMessageBox::warning(this, u8"����", u8"���ļ�ʧ��");
    }
}

void YourMainWindow::on_decompressButton_clicked() {
    if (!selectedFilePath.isEmpty()) {
        ui.dialog->clear();
        QString compressedFilePath = selectedFilePath;
        QString outputPath = QFileDialog::getSaveFileName(this, u8"�����ѹ�ļ�", "", "Text Files (*.txt)");
        if (!compressedFilePath.isEmpty()) {
            // ���� HuffmanCompressor ����
            HuffmanCompressor huffmanCompressor;
            // ִ�н�ѹ������
            ui.dialog->setText(u8"�ļ���ѹ��...\n");
            huffmanCompressor_raw = huffmanCompressor;
            huffmanCompressor_raw.decompressFile(compressedFilePath, outputPath);
            if (!outputPath.isEmpty()) {
                // ��ʾ�ɹ���Ϣ������
                QMessageBox::information(this, u8"��ѹ���ļ�", u8"�ļ���ѹ���ɹ���\n��ѹ���ļ�λ�ڣ�" + outputPath);
                ui.lineEditOutput->setText(outputPath);
                ui.dialog->setText("decompressed successfully!\n");
            }
            else {
                QMessageBox::warning(this, u8"��ѹ���ļ�", u8"��ѹʧ�ܻ�����ļ�δ��ѡ��");
            }
        }
        else {
            QMessageBox::warning(this, u8"����", u8"δ���ļ�");
        }
    }
}

void YourMainWindow::on_showEncodingTableButton_clicked() {
    // ����һ���µĴ���
    QDialog* encodingTableDialog = new QDialog(this);
    encodingTableDialog->setWindowTitle(u8"�����");

    // ����һ���ı���������ʾ�����
    QTextEdit* textEdit = new QTextEdit(encodingTableDialog);
    qDebug() << "Encoding Table:" << huffmanCompressor_raw.getEncodingTable();
    textEdit->setPlainText(huffmanCompressor_raw.getEncodingTable()); // getEncodingTable() ���ر������ַ���
    textEdit->setReadOnly(true);

    // ���ֹ���
    QVBoxLayout* layout = new QVBoxLayout(encodingTableDialog);
    layout->addWidget(textEdit);
    encodingTableDialog->setLayout(layout);

    // ��ʾ����
    encodingTableDialog->exec();
}


YourMainWindow::~YourMainWindow()
{}
