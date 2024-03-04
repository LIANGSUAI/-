// huffmantree.h

#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <QMap>
#include <Qtextstream>

class HuffmanNode {
public:
    QChar character;
    int frequency;
    HuffmanNode* leftChild;
    HuffmanNode* rightChild;

    HuffmanNode(QChar character, int frequency);
    bool HuffmanNode::isLeaf() const;
    HuffmanNode* HuffmanNode::getLeftChild() const;
    HuffmanNode* HuffmanNode::getRightChild() const;
    QString HuffmanNode::toString() const;
    QChar getCharacter() const;
};

class HuffmanTree {
private:
    HuffmanNode* root;
    void buildHuffmanTree(const QMap<QChar, int>& frequencyMap);
    void deleteHuffmanTree(HuffmanNode* node);
    void getHuffmanCodesRecursive(const HuffmanNode* node, QString& currentCode, QMap<QChar, QString>& huffmanCodes) const;
public:
    HuffmanTree(const QMap<QChar, int>& frequencyMap);
    HuffmanTree();
    ~HuffmanTree();
    QMap<QChar, QString> getHuffmanCodes() const;
    HuffmanNode* getRoot() { return root; }
    QString compress(const QString& input) const;
    QString decompress(const QString& compressedData, const QMap<QChar, QString>& encodingTableMap);
    QString toString() const;
    void HuffmanTree::buildStringRepresentation(const HuffmanNode* node, QString& result) const;
    void HuffmanTree::buildHuffmanTreeFromEncodingTable(const QString& encodingTable);
    void HuffmanTree::buildTreeFromCode(QChar character, const QString& code);
    void HuffmanTree::clear();
    void HuffmanTree::clearHelper(HuffmanNode* node);
    void HuffmanTree::printTree(HuffmanNode* root, int depth);
};

#endif // HUFFMANTREE_H
