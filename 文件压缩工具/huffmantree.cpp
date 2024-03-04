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
    // 构建哈夫曼树的逻辑
    QQueue<HuffmanNode*> nodeQueue;
    // 初始化节点队列
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        HuffmanNode* node = new HuffmanNode(it.key(), it.value());
        nodeQueue.enqueue(node);
    }
    // 构建哈夫曼树
    while (nodeQueue.size() > 1) {
        // 取出两个频率最小的节点
        HuffmanNode* left = nodeQueue.dequeue();
        HuffmanNode* right = nodeQueue.dequeue();
        // 创建新的父节点，频率为左右子节点频率之和
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->leftChild = left;
        parent->rightChild = right;
        // 将新节点放回队列
        nodeQueue.enqueue(parent);
    }
    // 最后队列中只剩一个节点，即根节点
    root = nodeQueue.dequeue();
}


void HuffmanTree::deleteHuffmanTree(HuffmanNode* node) {
    // 递归删除哈夫曼树的节点
    if (node) {
        deleteHuffmanTree(node->leftChild);
        deleteHuffmanTree(node->rightChild);
        delete node;
    }
}

void HuffmanTree::getHuffmanCodesRecursive(const HuffmanNode* node, QString& currentCode, QMap<QChar, QString>& huffmanCodes) const {
    // 递归获取哈夫曼编码
    if (node) {
        if (node->character != '\0') {
            huffmanCodes[node->character] = currentCode;
        }
        currentCode.append('0');
        getHuffmanCodesRecursive(node->leftChild, currentCode, huffmanCodes);
        currentCode.chop(1);  // 移除最后一个字符

        currentCode.append('1');
        getHuffmanCodesRecursive(node->rightChild, currentCode, huffmanCodes);
        currentCode.chop(1);  // 移除最后一个字符
    }
}

void HuffmanTree::buildHuffmanTreeFromEncodingTable(const QString& encodingTable) {
    // 清空现有的哈夫曼树
    clear();
    // 按行拆分编码表
    QStringList entries = encodingTable.split('\t', QString::SkipEmptyParts);
    for (const QString& entry : entries) {
        // 按 ": " 分割每个条目
        QStringList parts = entry.split(": ", QString::SkipEmptyParts);
        if (parts.size() == 2) {
            QChar character = parts[0][0];
            QString binaryCode = parts[1];
            // 根据编码构建哈夫曼树
            buildTreeFromCode(character, binaryCode);
        }
    }
}

void HuffmanTree::buildTreeFromCode(QChar character, const QString& code) {
    if (root == nullptr) {
        // 如果根节点为空，则创建一个根节点
        root = new HuffmanNode('\0', 0);
    }
    HuffmanNode* current = root;
    for (const QChar& bit : code) {
        if (bit == '0') {
            if (current->leftChild == nullptr) {
                // 如果左子节点为空，则创建一个新节点
                current->leftChild = new HuffmanNode('\0', 0);
            }
            current = current->leftChild;
        }
        else if (bit == '1') {
            if (current->rightChild == nullptr) {
                // 如果右子节点为空，则创建一个新节点
                current->rightChild = new HuffmanNode('\0', 0);
            }
            current = current->rightChild;
        }
    }
    // 在叶子节点处存储字符
    current->character = character;
}

void HuffmanTree::clearHelper(HuffmanNode* node) {
    if (node != nullptr) {
        // 递归删除左右子树
        clearHelper(node->leftChild);
        clearHelper(node->rightChild);

        // 删除当前节点
        delete node;
    }
}

void HuffmanTree::clear() {
    // 调用辅助函数，从根节点开始清空整个哈夫曼树
    clearHelper(root);

    // 将根节点置为 nullptr
    root = nullptr;
}

QMap<QChar, QString> HuffmanTree::getHuffmanCodes() const {
    // 获取字符对应的哈夫曼编码的逻辑
    QMap<QChar, QString> huffmanCodes;
    QString currentCode;
    getHuffmanCodesRecursive(root, currentCode, huffmanCodes);
    return huffmanCodes;
}

QString HuffmanTree::compress(const QString& input) const {
    // 压缩逻辑
    QMap<QChar, QString> huffmanCodes = getHuffmanCodes();
    QString compressed;
    for (const QChar& character : input) {
        compressed += huffmanCodes[character];
    }

    return compressed;
}

QString HuffmanTree::decompress(const QString& compressedData, const QMap<QChar, QString>& encodingTableMap) {
    QString decompressedData;
    QString currentCode; // 用于存储当前正在解码的比特串
    for (int i = 0; i < compressedData.size(); ++i) {
        currentCode.append(compressedData.at(i));
        // 检查当前比特串是否匹配编码表中的某个字符的编码
        for (auto it = encodingTableMap.begin(); it != encodingTableMap.end(); ++it) {
            if (currentCode == it.value()) {
                if (it.key() == '\n') {
                    decompressedData.append('\n');
                }
                else {
                    decompressedData.append(it.key());
                }
                currentCode.clear(); // 清空当前比特串
                break; // 跳出循环，继续处理下一个比特
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
        // 如果是叶子节点，表示一个字符
        if (node->isLeaf()) {
            result += QString("'%1'").arg(node->character);
        }
        else {
            // 非叶子节点，递归构建左右子树的表示
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


