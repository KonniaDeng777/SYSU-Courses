from time import process_time
import tensorflow as tf
from keras.preprocessing.text import Tokenizer
from keras.utils import pad_sequences
from keras.layers import Embedding, Conv1D, MaxPooling1D, Flatten, Dropout, Dense
from keras.models import Sequential
from keras.callbacks import History
import matplotlib.pyplot as plt
from keras import regularizers
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from keras.optimizers import SGD, Adagrad
from data_process import *

# 设定参数
embedding_dim = 300
max_length = 200
trunc_type = 'post'
padding_type = 'post'
oov_tok = '<OOV>'
vocab_size = 5000
batch_size = 32
epochs = 20  # 训练次数

embeddings_index = load_embeddings('data/glove.6B.300d.txt')
labels = load_labels('data/20ns/labels.txt')

tf.config.list_physical_devices('GPU')
start = process_time()
train_texts, train_targets = load_data('data/20ns/train.txt', labels)
valid_texts, valid_targets = load_data('data/20ns/valid.txt', labels)
test_texts, test_targets = load_data('data/20ns/test.txt', labels)

# 创建分词器
tokenizer = Tokenizer(num_words=vocab_size, oov_token=oov_tok)
tokenizer.fit_on_texts(train_texts)

# 文本转为序列
train_sequences = tokenizer.texts_to_sequences(train_texts)
valid_sequences = tokenizer.texts_to_sequences(valid_texts)
test_sequences = tokenizer.texts_to_sequences(test_texts)

# 序列填充
train_padded = pad_sequences(train_sequences, maxlen=max_length, padding=padding_type, truncating=trunc_type)
valid_padded = pad_sequences(valid_sequences, maxlen=max_length, padding=padding_type, truncating=trunc_type)
test_padded = pad_sequences(test_sequences, maxlen=max_length, padding=padding_type, truncating=trunc_type)

# 创建词嵌入矩阵
embedding_matrix = np.zeros((vocab_size, embedding_dim))
for word, i in tokenizer.word_index.items():
    if i < vocab_size:
        embedding_vector = embeddings_index.get(word)
        if embedding_vector is not None:
            embedding_matrix[i] = embedding_vector

# 构建CNN模型
# 模型1
# model = Sequential([
#     Embedding(vocab_size, embedding_dim, input_length=max_length, weights=[embedding_matrix], trainable=False),
#     Conv1D(128, 5, activation='relu'),
#     MaxPooling1D(5),
#     Flatten(),
#     Dropout(0.5),
#     Dense(64, activation='relu'),
#     Dense(len(labels), activation='softmax')
# ])
# 模型2
model = Sequential([
    Embedding(vocab_size, embedding_dim, input_length=max_length, weights=[embedding_matrix], trainable=False),
    Conv1D(128, 5, activation='relu'),
    MaxPooling1D(5),
    Conv1D(64, 5, activation='relu'),
    MaxPooling1D(5),
    Flatten(),
    Dropout(0.5),
    Dense(64, activation='relu'),  # 全连接层
    Dense(len(labels), activation='softmax')
])
# 模型3
# model = Sequential([
#     Embedding(vocab_size, embedding_dim, input_length=max_length, weights=[embedding_matrix], trainable=False),
#     Conv1D(256, 5, activation='relu'),
#     MaxPooling1D(3),
#     Conv1D(128, 5, activation='relu'),
#     MaxPooling1D(3),
#     Conv1D(64, 5, activation='relu'),
#     MaxPooling1D(3),
#     Flatten(),
#     Dropout(0.5),
#     Dense(64, activation='relu'),
#     Dense(len(labels), activation='softmax')
# ])


history = History()
sgd = SGD(learning_rate=0.01, decay=1e-6, momentum=0.9, nesterov=True)
adagrad = Adagrad(learning_rate=0.01, epsilon=1e-6)
model.compile(loss='sparse_categorical_crossentropy', optimizer='rmsprop', metrics=['accuracy'])
# 训练模型
model.fit(train_padded, np.array(train_targets), batch_size=batch_size, epochs=epochs,
          validation_data=(valid_padded, np.array(valid_targets)), callbacks=[history])

print('')
# 输出验证集和测试集的准确率
valid_loss, valid_accuracy = model.evaluate(valid_padded, np.array(valid_targets), batch_size=batch_size)
print("Validation accuracy: {:.2f}%".format(valid_accuracy * 100))
print('')
test_loss, test_accuracy = model.evaluate(test_padded, np.array(test_targets), batch_size=batch_size)
print("Test accuracy: {:.2f}%".format(test_accuracy * 100))
end = process_time()
print(f'processing time: {end - start} seconds')

# plot
plt.plot(history.history['accuracy'])
plt.plot(history.history['val_accuracy'])
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend(['test', 'validation'], loc='upper right')
plt.grid(True)
plt.show()
