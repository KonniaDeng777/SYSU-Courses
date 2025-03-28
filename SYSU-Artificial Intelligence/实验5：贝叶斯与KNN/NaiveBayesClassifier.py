import csv
from collections import defaultdict
import math
from time import process_time
import nltk
from nltk.corpus import stopwords

import matplotlib.pyplot as plt


class NaiveBayesClassifier:
    def __init__(self, categories):
        self.categories = categories
        self.word_counts = {category: defaultdict(int) for category in categories}
        self.category_counts = {category: 0 for category in categories}

    # 多项式模型
    def train(self, text, category):
        for word in text.split():
            self.word_counts[category][word] += 1  # 类别为category的文本中出现word的次数
            self.category_counts[category] += 1  # 类别为category的文本中的单词总数

    def predict(self, text, alpha=1e-3):
        # stop_words = set(stopwords.words('english'))
        words = text.split()
        # words = [word for word in words if word not in stop_words]
        log_probs = {}
        total_count = sum(self.category_counts.values())  # 单词总数
        for category in self.categories:
            log_prob = math.log(self.category_counts[category] / total_count)
            for word in words:
                word_count = self.word_counts[category][word] + alpha  # Laplace smoothing
                # log_prob += math.log(word_count / (self.category_counts[category] + len(self.word_counts[category])*alpha))
                log_prob += math.log(word_count / (self.category_counts[category] + len(self.word_counts[category])))
                # len(self.word_counts[category]):类别为category的文本中不重复的单词总数
                # category_counts[category]:类别为category的文本中的所有单词总数
            log_probs[category] = log_prob
        return max(log_probs, key=log_probs.get)


def read_data(filename):
    data = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # 跳过标题
        for row in reader:
            text, label = row
            data.append((text, label))
    return data


def main():
    categories = ['joy', 'sad', 'anger', 'fear', 'surprise', 'disgust']

    # File
    train_file = 'data/train_set.csv'
    validation_file = 'data/validation_set.csv'
    test_file = 'data/classification_test_set.csv'
    result_file = 'data/21307035_dengxuying_NB_classification.csv'

    # Train
    train_data = read_data(train_file)
    classifier = NaiveBayesClassifier(categories)
    for text, label in train_data:
        classifier.train(text, label)

    alpha = 1e-3
    # # plot accuracy
    # index=1
    # index_set = []
    # accuracy = []
    # accuracy_max = 0.0
    # accuracy_max_index = 1e-11
    # validation_data = read_data(validation_file)
    # while alpha <= 1:
    #     validation_predictions = []
    #     correct = 0
    #     for text, label in validation_data:
    #         predicted_label = classifier.predict(text, alpha)
    #         validation_predictions.append((text, predicted_label))
    #         if predicted_label == label:
    #             correct += 1
    #     validation_accuracy = correct / len(validation_data)
    #     if (accuracy_max < validation_accuracy):
    #         accuracy_max = max(accuracy_max, validation_accuracy)
    #         accuracy_max_index = alpha
    #     accuracy.append(validation_accuracy)
    #     index_set.append(index)
    #     index+=1
    #     alpha = alpha * 5
    # print(accuracy_max_index)
    # print(accuracy_max)
    # print(accuracy)
    # plt.plot(index_set, accuracy)
    # # plt.xscale('log')
    # plt.xlabel('index')
    # plt.ylabel('accuracy')
    # plt.title('validation_accuracy')
    # plt.grid()
    # plt.show()

    # Validate
    validation_predictions = []
    validation_data = read_data(validation_file)
    correct = 0
    for text, label in validation_data:
        predicted_label = classifier.predict(text, alpha)
        validation_predictions.append((text, predicted_label))
        if predicted_label == label:
            correct += 1
    print('Validation accuracy:', correct / len(validation_data))

    # Test
    test_data = read_data(test_file)
    predictions = []
    test_correct = 0
    for text, label in test_data:
        predicted_label = classifier.predict(text, alpha)
        predictions.append((id, text, predicted_label))
        if predicted_label == label:
            test_correct += 1
    print('Test accuracy:', test_correct / len(test_data))

    # Write answers
    with open(result_file, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Words (split by space)', 'label'])
        for _, text, predicted_label in predictions:
            writer.writerow([text, predicted_label])


if __name__ == '__main__':
    start_time = process_time()
    main()
    end_time = process_time()
    print(f'processing time: {end_time - start_time} seconds')
