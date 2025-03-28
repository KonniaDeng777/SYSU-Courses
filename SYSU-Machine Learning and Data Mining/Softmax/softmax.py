import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.optimizers import SGD, Adam
import matplotlib.pyplot as plt
from keras.datasets import cifar10

# 加载CIFAR-10数据集
(x_train, y_train), (x_test, y_test) = cifar10.load_data()

# 数据预处理
x_train = x_train.astype("float32") / 255.0
x_test = x_test.astype("float32") / 255.0

# 将标签进行独热编码
num_classes = 10
y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)

# 创建模型
model = keras.Sequential([
    layers.Flatten(input_shape=(32, 32, 3)),
    layers.Dense(num_classes, activation="softmax")
])

sgd_optimizer = SGD(learning_rate=0.001)
sgd_momentum_optimizer = SGD(learning_rate=0.001, momentum=0.9)
adam_optimizer = Adam(learning_rate=0.001)

# 编译模型
model.compile(optimizer=sgd_optimizer, loss="categorical_crossentropy", metrics=["accuracy"])

# 训练模型
history_sgd = model.fit(x_train, y_train, batch_size=64, epochs=10, validation_data=(x_test, y_test))

# 使用SGD Momentum算法训练模型
model.compile(optimizer=sgd_momentum_optimizer, loss="categorical_crossentropy", metrics=["accuracy"])
history_momentum = model.fit(x_train, y_train, batch_size=64, epochs=10, validation_data=(x_test, y_test))

# 使用Adam算法训练模型
model.compile(optimizer=adam_optimizer, loss="categorical_crossentropy", metrics=["accuracy"])
history_adam = model.fit(x_train, y_train, batch_size=64, epochs=10, validation_data=(x_test, y_test))

# 画图比较不同优化算法的性能
plt.plot(history_sgd.history["val_accuracy"], label="SGD")
plt.plot(history_momentum.history["val_accuracy"], label="SGD Momentum")
plt.plot(history_adam.history["val_accuracy"], label="Adam")
plt.title("Model Accuracy")
plt.xlabel("Epoch")
plt.ylabel("Accuracy")
plt.legend()
plt.savefig('softmax.png')
plt.show()