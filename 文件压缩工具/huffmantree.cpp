// huffmantree.cpp

#include "huffmantree.h"
#include <QQueue>
#include <QMap>
#include <QBitArray>
#include <QDebug>
#include <iomanip>
#include <iostream>

HuffmanNode::HuffmanNode(QChar character, int frequency)
    : character(character), frequency(frequency), leftChild(nullptr), rightChild(nullptr) {}

HuffmanTree::HuffmanTree(const QMap<QChar, int>& frequencyMap) : root(nullptr) {
    buildHuffmanTree(frequencyMap);
}

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    deleteHuffmanTree(root);
}

void HuffmanTree::printTree(HuffmanNode* root, int depth) {
    if (root != nullptr) {
        printTree(root->rightChild, depth + 1);
        for (int i = 0; i < 4 * depth; i++) {
            qDebug() << " ";
        }
        qDebug() << root->character << endl;
        printTree(root->leftChild, depth + 1);
    }
}

void HuffmanTree::buildHuffmanTree(const QMap<QChar, int>& frequencyMap) {
    // ���������������߼�
    QQueue<HuffmanNode*> nodeQueue;
    // ��ʼ���ڵ����
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        HuffmanNode* node = new HuffmanNode(it.key(), it.value());
        nodeQueue.enqueue(node);
    }
    // ������������
    while (nodeQueue.size() > 1) {
        // ȡ������Ƶ����С�Ľڵ�
        HuffmanNode* left = nodeQueue.dequeue();
        HuffmanNode* right = nodeQueue.dequeue();
        // �����µĸ��ڵ㣬Ƶ��Ϊ�����ӽڵ�Ƶ��֮��
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->leftChild = left;
        parent->rightChild = right;
        // ���½ڵ�Żض���
        nodeQueue.enqueue(parent);
    }
    // ��������ֻʣһ���ڵ㣬�����ڵ�
    root = nodeQueue.dequeue();
}


void HuffmanTree::deleteHuffmanTree(HuffmanNode* node) {
    // �ݹ�ɾ�����������Ľڵ�
    if (node) {
        deleteHuffmanTree(node->leftChild);
        deleteHuffmanTree(node->rightChild);
        delete node;
    }
}

void HuffmanTree::getHuffmanCodesRecursive(const HuffmanNode* node, QString& currentCode, QMap<QChar, QString>& huffmanCodes) const {
    // �ݹ��ȡ����������
    if (node) {
        if (node->character != '\0') {
            huffmanCodes[node->character] = currentCode;
        }
        currentCode.append('0');
        getHuffmanCodesRecursive(node->leftChild, currentCode, huffmanCodes);
        currentCode.chop(1);  // �Ƴ����һ���ַ�

        currentCode.append('1');
        getHuffmanCodesRecursive(node->rightChild, currentCode, huffmanCodes);
        currentCode.chop(1);  // �Ƴ����һ���ַ�
    }
}

void HuffmanTree::buildHuffmanTreeFromEncodingTable(const QString& encodingTable) {
    // ������еĹ�������
    clear();
    // ���в�ֱ����
    QStringList entries = encodingTable.split('\t', QString::SkipEmptyParts);
    for (const QString& entry : entries) {
        // �� ": " �ָ�ÿ����Ŀ
        QStringList parts = entry.split(": ", QString::SkipEmptyParts);
        if (parts.size() == 2) {
            QChar character = parts[0][0];
            QString binaryCode = parts[1];
            // ���ݱ��빹����������
            buildTreeFromCode(character, binaryCode);
        }
    }
}

void HuffmanTree::buildTreeFromCode(QChar character, const QString& code) {
    if (root == nullptr) {
        // ������ڵ�Ϊ�գ��򴴽�һ�����ڵ�
        root = new HuffmanNode('\0', 0);
    }
    HuffmanNode* current = root;
    for (const QChar& bit : code) {
        if (bit == '0') {
            if (current->leftChild == nullptr) {
                // ������ӽڵ�Ϊ�գ��򴴽�һ���½ڵ�
                current->leftChild = new HuffmanNode('\0', 0);
            }
            current = current->leftChild;
        }
        else if (bit == '1') {
            if (current->rightChild == nullptr) {
                // ������ӽڵ�Ϊ�գ��򴴽�һ���½ڵ�
                current->rightChild = new HuffmanNode('\0', 0);
            }
            current = current->rightChild;
        }
    }
    // ��Ҷ�ӽڵ㴦�洢�ַ�
    current->character = character;
}

void HuffmanTree::clearHelper(HuffmanNode* node) {
    if (node != nullptr) {
        // �ݹ�ɾ����������
        clearHelper(node->leftChild);
        clearHelper(node->rightChild);

        // ɾ����ǰ�ڵ�
        delete node;
    }
}

void HuffmanTree::clear() {
    // ���ø����������Ӹ��ڵ㿪ʼ���������������
    clearHelper(root);

    // �����ڵ���Ϊ nullptr
    root = nullptr;
}

QMap<QChar, QString> HuffmanTree::getHuffmanCodes() const {
    // ��ȡ�ַ���Ӧ�Ĺ�����������߼�
    QMap<QChar, QString> huffmanCodes;
    QString currentCode;
    getHuffmanCodesRecursive(root, currentCode, huffmanCodes);
    return huffmanCodes;
}

QString HuffmanTree::compress(const QString& input) const {
    // ѹ���߼�
    QMap<QChar, QString> huffmanCodes = getHuffmanCodes();
    QString compressed;
    for (const QChar& character : input) {
        compressed += huffmanCodes[character];
    }

    return compressed;
}

QString HuffmanTree::decompress(const QString& compressedData, const QMap<QChar, QString>& encodingTableMap) {
    QString decompressedData;
    QString currentCode; // ���ڴ洢��ǰ���ڽ���ı��ش�
    for (int i = 0; i < compressedData.size(); ++i) {
        currentCode.append(compressedData.at(i));
        // ��鵱ǰ���ش��Ƿ�ƥ�������е�ĳ���ַ��ı���
        for (auto it = encodingTableMap.begin(); it != encodingTableMap.end(); ++it) {
            if (currentCode == it.value()) {
                if (it.key() == '\n') {
                    decompressedData.append('\n');
                }
                else {
                    decompressedData.append(it.key());
                }
                currentCode.clear(); // ��յ�ǰ���ش�
                break; // ����ѭ��������������һ������
            }
        }
    }
    return decompressedData;
}



bool HuffmanNode::isLeaf() const {
    return leftChild == nullptr && rightChild == nullptr;
}
HuffmanNode* HuffmanNode::getLeftChild() const {
    return leftChild;
}

HuffmanNode* HuffmanNode::getRightChild() const {
    return rightChild;
}
QString HuffmanNode::toString() const {
    QString result;
    if (isLeaf()) {
        result += QString("Leaf: Character %1, Frequency %2").arg(character).arg(frequency);
    }
    else {
        result += QString("Internal Node: Frequency %1").arg(frequency);
    }
    return result;
}
QString HuffmanTree::toString() const {
    QString result;
    buildStringRepresentation(root, result);
    return result; 
}
void HuffmanTree::buildStringRepresentation(const HuffmanNode* node, QString& result) const {
    if (node) {
        // �����Ҷ�ӽڵ㣬��ʾһ���ַ�
        if (node->isLeaf()) {
            result += QString("'%1'").arg(node->character);
        }
        else {
            // ��Ҷ�ӽڵ㣬�ݹ鹹�����������ı�ʾ
            result += "(";
            buildStringRepresentation(node->leftChild, result);
            result += ",";
            buildStringRepresentation(node->rightChild, result);
            result += ")";
        }
    }
}
QChar HuffmanNode::getCharacter() const {
    return character;
}


