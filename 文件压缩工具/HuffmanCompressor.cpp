// HuffmanCompressor.cpp

#include "HuffmanCompressor.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QBitArray>
#include <QDataStream>
#include <QDebug>
#include <QString>
#include <QMessageBox>

HuffmanCompressor::HuffmanCompressor() {}
HuffmanCompressor::HuffmanCompressor(const QMap<QChar, int>& frequencyMap) : huffmanTree(frequencyMap) {
    // ���������
    buildEncodingTable(huffmanTree.getRoot(), "");

    // ����Ҳ���Դ�ӡ��������ڵ���
    qDebug() << "Encoding Table:";
    for (auto it = encodingTable.begin(); it != encodingTable.end(); ++it) {
        qDebug() << it.key() << ": " << it.value();
    }
}

void HuffmanCompressor::setEncodingTable(QMap<QChar, QString> EncodingTable) {
    encodingTable = EncodingTable;
}

QString HuffmanCompressor::compressFile(const QString& inputFile, const QString& outputFile) {
    // ִ��ѹ���߼����������Ķ���������д�� outputFile
        // �������ļ��Թ���ȡ
    QFile inFile(inputFile);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // �����ȡ�����ļ�����
        QString errorMessage = "Error opening input file for reading: " + inFile.errorString();
        QMessageBox::critical(nullptr, "Error", errorMessage);
        return "";
    }
    // ������ļ��Թ�д��ѹ������
    QFile outFile(outputFile);
    if (!outFile.open(QIODevice::WriteOnly)) {
        // ����д������ļ�����
        QString errorMessage = "Error opening output file for writing: " + outFile.errorString();
        QMessageBox::critical(nullptr, "Error", errorMessage);
        return "";
    }
    // ���� QDataStream �Զ���������д������ļ�
    QDataStream outStream(&outFile);
    // д�������������Ϣ
    QString encodingTable = getEncodingTable();
    qDebug() << encodingTable;
    outStream << encodingTable;
    // ��ȡ�����ļ�����
    QString content = inFile.readAll();

    QString compressedContent = huffmanTree.compress(content);
    // ת���ַ���Ϊ QBitArray
    QBitArray compressedBits(compressedContent.size());
    for (int i = 0; i < compressedContent.size(); ++i) {
        compressedBits.setBit(i, compressedContent.at(i) == '1');
    }
    // ��λ����д������ļ�
    outStream << compressedBits;

    // �ر��ļ�
    inFile.close();
    outFile.close();

    // ����ѹ������ļ�·��
    return outputFile;
}

QString HuffmanCompressor::getEncodingTable() const {
    // ���ر������ַ�����ʾ
    QString result;
    for (auto it = encodingTable.begin(); it != encodingTable.end(); ++it) {
        QString character = it.key();
        QString code = it.value();
        result += QString("%1: %2\t").arg(character).arg(code);
    }
    return result;
}

QString HuffmanCompressor::getHuffmanTree() const {
    // ���ع����������ַ�����ʾ
    return huffmanTree.toString();
}

void HuffmanCompressor::buildEncodingTable(const HuffmanNode* node, const QString& currentCode) {
    if (node->isLeaf()) {
        encodingTable[node->getCharacter()] = currentCode;
    }
    else {
        if (node->getLeftChild()) {
            buildEncodingTable(node->getLeftChild(), currentCode + "0");
        }
        if (node->getRightChild()) {
            buildEncodingTable(node->getRightChild(), currentCode + "1");
        }
    }
}


QMap<QChar, QString> readEncodingTable(const QString& encodingTable) {
    QMap<QChar, QString> result;
    // �ָ�ÿһ��
    QStringList lines = encodingTable.split("\t", QString::SkipEmptyParts);
    for (const QString& line : lines) {
        // �ָ�ÿ�е��ַ��ͱ���
        QStringList parts = line.split(": ");
        if (parts.size() == 2) {
            QChar character = parts[0][0];
            QString code = parts[1];

            result.insert(character, code);
        }
    }
    return result;
}

void HuffmanCompressor::decompressFile(const QString& inputFile, const QString& outputFile) {
    QFile inFile(inputFile);
    if (inFile.open(QIODevice::ReadOnly)) {
        QDataStream inStream(&inFile);
        // ��ȡ������������Ϣ
        QString encodingTable;
        inStream >> encodingTable;
        qDebug() << encodingTable;
        QMap<QChar, QString> encodingTableMap = readEncodingTable(encodingTable);
        setEncodingTable(encodingTableMap);
        // ʹ�ñ��������������
        HuffmanTree huffmanTree;
        huffmanTree.buildHuffmanTreeFromEncodingTable(encodingTable);
        qDebug() << "Huffman Tree before decompression: " << huffmanTree.toString().toUtf8().constData();
        // ��������ļ�
        QFile outFile(outputFile);
        if (outFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&outFile);
            // ��ȡѹ���ļ�����
            QBitArray compressedBits;
            inStream >> compressedBits;
            // �� QBitArray ת��Ϊ�ַ���
            QString compressedContent;
            for (int i = 0; i < compressedBits.size(); ++i) {
                compressedContent.append(compressedBits.testBit(i) ? '1' : '0');
            }
            qDebug() << compressedContent;
           // ʹ�ù����������н�ѹ��
            QString decompressedData = huffmanTree.decompress(compressedContent, encodingTableMap);
            // ����ѹ���������д������ļ�
            out << decompressedData;
            qDebug() << decompressedData;
            outFile.close();
        }
        inFile.close();
    }
}


