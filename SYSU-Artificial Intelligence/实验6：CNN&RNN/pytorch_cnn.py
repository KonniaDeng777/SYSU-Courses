import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import pytest


# 定义CNN模型
class CNN(nn.Module):
    def __init__(self, vocab_size, embedding_dim, num_classes, pretrained_embeddings):
        super(CNN, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embedding_dim)
        self.embedding.weight.data.copy_(pretrained_embeddings)
        self.conv = nn.Conv1d(embedding_dim, 128, kernel_size=5)
        self.pool = nn.MaxPool1d(kernel_size=2)
        self.fc = nn.Linear(128 * ((4096 - 4) // 2), num_classes)

    def forward(self, x):
        x = self.embedding(x)
        x = x.transpose(1, 2)
        x = self.conv(x)
        x = self.pool(x)
        x = x.squeeze(dim=2)
        batch_size = x.size(0)
        x = x.view(batch_size, -1)  # 将张量展平为二维张量
        fc_weight = self.fc.weight.view(num_classes, -1)
        input_dim = x.size(1)
        if input_dim != fc_weight.size(1):
            self.fc = nn.Linear(input_dim, num_classes)
        x = self.fc(x)
        return x


# 定义词表处理函数
def process_text(text):
    # 去除标点符号和数字
    text = ''.join(c for c in text if c.isalpha() or c.isspace())
    # 将文本转换为小写
    text = text.lower()
    # 将文本分词
    words = text.split()
    return words


# 读取预训练的词嵌入数据
def load_embeddings(embeddings_file):
    with open(embeddings_file, 'r') as f:
        lines = f.readlines()
        embeddings = {}
        for line in lines:
            values = line.strip().split()
            word = values[0]
            vector = np.array(values[1:], dtype='float32')
            embeddings[word] = vector
    return embeddings


# 加载训练集、验证集和测试集
def load_data(data_file):
    with open(data_file, 'r') as f:
        lines = f.readlines()
        data = []
        for line in lines:
            label, text = line.strip().split('\t')
            words = process_text(text)
            data.append((label, words))
    return data


# 构建词汇表
def build_vocab(data):
    vocab = {}
    for label, words in data:
        for word in words:
            if word not in vocab:
                vocab[word] = len(vocab)
    return vocab


# 将文本转换为数字序列
def text_to_sequence(text, vocab):
    seq = [vocab[word] for word in text if word in vocab]
    return seq


# 将数据转换为 PyTorch 张量
# 将数据转换为 PyTorch 张量
def data_to_tensor(data, vocab):
    labels = []
    sequences = []
    label_map = []
    max_length = 0
    for label, words in data:
        if label not in label_map:
            label_map.append(label)
        labels.append(label_map.index(label))
        seq = text_to_sequence(words, vocab)
        sequences.append(seq)
        max_length = max(max_length, len(seq))
    # 填充序列数据为相同的长度
    for i in range(len(sequences)):
        sequences[i] += [0] * (max_length - len(sequences[i]))
    labels_tensor = torch.tensor(labels, dtype=torch.long)
    sequences_tensor = torch.LongTensor(sequences)
    return labels_tensor, sequences_tensor


# 训练函数
def train(model, optimizer, criterion, train_data, valid_data, vocab):
    # 定义训练参数
    num_epochs = 10
    batch_size = 32

    # 将数据转换为 PyTorch 张量
    train_labels, train_sequences = data_to_tensor(train_data, vocab)
    valid_labels, valid_sequences = data_to_tensor(valid_data, vocab)

    # 训练循环
    for epoch in range(num_epochs):
        # 将训练数据打乱并分成批次
        permutation = torch.randperm(train_sequences.size()[0])
        for i in range(0, train_sequences.size()[0], batch_size):
            indices = permutation[i:i + batch_size]
            batch_labels = train_labels[indices]
            batch_sequences = train_sequences[indices]

            # 计算模型的预测值
            outputs = model(batch_sequences)

            # 计算损失并进行反向传播
            loss = criterion(outputs, batch_labels)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

        # 在验证集上进行验证并输出准确率
        with torch.no_grad():
            valid_outputs = model(valid_sequences)
            _, valid_predicted = torch.max(valid_outputs.data, 1)
            valid_accuracy = torch.sum(valid_predicted == valid_labels).item() / valid_labels.size()[0]
            print('Epoch [{}/{}], Valid Accuracy: {:.4f}'.format(epoch + 1, num_epochs, valid_accuracy))


# 测试函数
def cnntest(model, test_data, vocab):
    # 将数据转换为 PyTorch 张量
    test_labels, test_sequences = data_to_tensor(test_data, vocab)

    # 在测试集上进行测试并输出准确率
    with torch.no_grad():
        test_outputs = model(test_sequences)
        _, test_predicted = torch.max(test_outputs.data, 1)
        test_accuracy = torch.sum(test_predicted == test_labels).item() / test_labels.size()[0]
        print('Test Accuracy: {:.4f}'.format(test_accuracy))


# 主函数
if __name__ == '__main__':
    # 加载数据
    train_data = load_data('data/20ns/train.txt')
    valid_data = load_data('data/20ns/valid.txt')
    test_data = load_data('data/20ns/test.txt')
    labels = set(label for label, _ in train_data + valid_data + test_data)

    # 构建词汇表
    vocab = build_vocab(train_data + valid_data + test_data)

    # 加载预训练的词嵌入数据
    embeddings = load_embeddings('data/glove.6b.50d.txt')

    # 构建词嵌入矩阵
    embedding_dim = 50
    pretrained_embeddings = np.zeros((len(vocab), embedding_dim))
    for word, i in vocab.items():
        if word in embeddings:
            pretrained_embeddings[i] = embeddings[word]

    # 定义模型和优化器
    num_classes = len(labels)
    model = CNN(len(vocab), embedding_dim, num_classes, torch.from_numpy(pretrained_embeddings))
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters())

    # 进行训练和测试
    train(model, optimizer, criterion, train_data, valid_data, vocab)
    cnntest(model, test_data, vocab)
