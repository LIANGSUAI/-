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
    // 构建编码表
    buildEncodingTable(huffmanTree.getRoot(), "");

    // 这里也可以打印编码表，便于调试
    qDebug() << "Encoding Table:";
    for (auto it = encodingTable.begin(); it != encodingTable.end(); ++it) {
        qDebug() << it.key() << ": " << it.value();
    }
}

void HuffmanCompressor::setEncodingTable(QMap<QChar, QString> EncodingTable) {
    encodingTable = EncodingTable;
}

QString HuffmanCompressor::compressFile(const QString& inputFile, const QString& outputFile) {
    // 执行压缩逻辑，将编码后的二进制数据写入 outputFile
        // 打开输入文件以供读取
    QFile inFile(inputFile);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 处理读取输入文件错误
        QString errorMessage = "Error opening input file for reading: " + inFile.errorString();
        QMessageBox::critical(nullptr, "Error", errorMessage);
        return "";
    }
    // 打开输出文件以供写入压缩数据
    QFile outFile(outputFile);
    if (!outFile.open(QIODevice::WriteOnly)) {
        // 处理写入输出文件错误
        QString errorMessage = "Error opening output file for writing: " + outFile.errorString();
        QMessageBox::critical(nullptr, "Error", errorMessage);
        return "";
    }
    // 创建 QDataStream 以二进制数据写入输出文件
    QDataStream outStream(&outFile);
    // 写入哈夫曼编码信息
    QString encodingTable = getEncodingTable();
    qDebug() << encodingTable;
    outStream << encodingTable;
    // 读取输入文件内容
    QString content = inFile.readAll();

    QString compressedContent = huffmanTree.compress(content);
    // 转换字符串为 QBitArray
    QBitArray compressedBits(compressedContent.size());
    for (int i = 0; i < compressedContent.size(); ++i) {
        compressedBits.setBit(i, compressedContent.at(i) == '1');
    }
    // 将位数据写入输出文件
    outStream << compressedBits;

    // 关闭文件
    inFile.close();
    outFile.close();

    // 返回压缩后的文件路径
    return outputFile;
}

QString HuffmanCompressor::getEncodingTable() const {
    // 返回编码表的字符串表示
    QString result;
    for (auto it = encodingTable.begin(); it != encodingTable.end(); ++it) {
        QString character = it.key();
        QString code = it.value();
        result += QString("%1: %2\t").arg(character).arg(code);
    }
    return result;
}

QString HuffmanCompressor::getHuffmanTree() const {
    // 返回哈夫曼树的字符串表示
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
    // 分割每一行
    QStringList lines = encodingTable.split("\t", QString::SkipEmptyParts);
    for (const QString& line : lines) {
        // 分割每行的字符和编码
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
        // 读取哈夫曼编码信息
        QString encodingTable;
        inStream >> encodingTable;
        qDebug() << encodingTable;
        QMap<QChar, QString> encodingTableMap = readEncodingTable(encodingTable);
        setEncodingTable(encodingTableMap);
        // 使用编码表构建哈夫曼树
        HuffmanTree huffmanTree;
        huffmanTree.buildHuffmanTreeFromEncodingTable(encodingTable);
        qDebug() << "Huffman Tree before decompression: " << huffmanTree.toString().toUtf8().constData();
        // 创建输出文件
        QFile outFile(outputFile);
        if (outFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&outFile);
            // 读取压缩文件内容
            QBitArray compressedBits;
            inStream >> compressedBits;
            // 将 QBitArray 转换为字符串
            QString compressedContent;
            for (int i = 0; i < compressedBits.size(); ++i) {
                compressedContent.append(compressedBits.testBit(i) ? '1' : '0');
            }
            qDebug() << compressedContent;
           // 使用哈夫曼树进行解压缩
            QString decompressedData = huffmanTree.decompress(compressedContent, encodingTableMap);
            // 将解压缩后的内容写入输出文件
            out << decompressedData;
            qDebug() << decompressedData;
            outFile.close();
        }
        inFile.close();
    }
}


