import re
import numpy as np


# 读取预训练的词嵌入数据
def load_embeddings(embedding_file):
    embeddings_index = {}
    with open(embedding_file, encoding='utf-8') as f:
        for line in f:
            value = line.split()
            word = value[0]
            coef = np.asarray(value[1:], dtype='float32')
            embeddings_index[word] = coef
    return embeddings_index


# 读取标签数据
def load_labels(label_file):
    with open(label_file, 'r') as f:
        labels = [label.strip() for label in f.readlines()]
    return labels


# 处理文本数据
def clean_text(text):
    # 将非字母、数字、标点替换成空格
    text = re.sub(r"[^A-Za-z0-9(),!?\'\`]", " ", text)
    text = re.sub(r'\d+', '', text)  # 去除数字
    text = re.sub(r'[^\w\s]', '', text)  # 去除标点
    # 去除转义字符
    text = re.sub(r"\'s", " 's", text)
    text = re.sub(r"\'ve", " 've", text)
    text = re.sub(r"n\'t", " n't", text)
    text = re.sub(r"\'re", " 're", text)
    text = re.sub(r"\'d", " 'd", text)
    text = re.sub(r"\'ll", " 'll", text)
    # 标点符号前后添加空格
    text = re.sub(r",", " , ", text)
    text = re.sub(r"!", " ! ", text)
    text = re.sub(r"\(", " ( ", text)
    text = re.sub(r"\)", " ) ", text)
    text = re.sub(r"\?", " ? ", text)
    # 将多余的空格替换成单个空格
    text = re.sub(r"\s{2,}", " ", text)
    # 去除首尾的空格，并全部转换为小写后返回
    return text.strip().lower()


def load_data(file, labels):
    texts = []
    targets = []
    with open(file, 'r', encoding='utf-8') as f:
        for line in f:
            label, text = line.strip().split('\t')
            text = clean_text(text)
            texts.append(text)
            targets.append(labels.index(label))
    return texts, targets
