#pragma once
#include "huffmantree.h"
class HuffmanCompressor {
public:
    HuffmanCompressor(const QMap<QChar, int>& frequencyMap);
    HuffmanCompressor();
    QString getEncodingTable() const;
    QString getHuffmanTree() const;
    QString compressFile(const QString& inputFile, const QString& outputFile);
    void decompressFile(const QString& inputFile, const QString& outputFile);
    void setEncodingTable(QMap<QChar, QString>);

private:
    HuffmanTree huffmanTree;
    QMap<QChar, QString> encodingTable;
    void buildEncodingTable(const HuffmanNode* node, const QString& currentCode);
};

