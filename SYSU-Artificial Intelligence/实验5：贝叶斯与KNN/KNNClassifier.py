import csv
import math
import operator
from collections import Counter
from time import process_time
import nltk
from nltk.corpus import stopwords
# nltk.download('stopwords')

from matplotlib import pyplot as plt


class KNNClassifier:
    def __init__(self, k=46):
        self.k = k
        self.train_data = []

    def fit(self, X, y):
        self.train_data = list(zip(X, y))

    def euclidean_distance(self, vec1, vec2):
        return math.sqrt(sum([(a - b) ** 2 for a, b in zip(vec1, vec2)]))

    def cosine_distance(self,vec1, vec2):
        dot_product = sum([a * b for a, b in zip(vec1, vec2)])
        magnitude1 = math.sqrt(sum([a ** 2 for a in vec1]))
        magnitude2 = math.sqrt(sum([b ** 2 for b in vec2]))
        return 1 - (dot_product / (magnitude1 * magnitude2))

    def jaccard_distance(slef,vec1, vec2):
        intersection = len(set(vec1).intersection(set(vec2)))
        union = len(set(vec1).union(set(vec2)))
        return 1 - (intersection / union)

    def predict(self, X):
        predictions = []
        for x in X:
            distances = [(i, self.euclidean_distance(x, train_x)) for i, (train_x, _) in enumerate(self.train_data)]
            distances.sort(key=operator.itemgetter(1))
            k_nearest_neighbors = [self.train_data[i][1] for i, _ in distances[:self.k]]
            predicted_label = Counter(k_nearest_neighbors).most_common(1)[0][0]
            predictions.append(predicted_label)
        return predictions


def read_data(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        next(reader)
        data = []
        labels = []
        for row in reader:
            data.append(row[0].split())
            labels.append(row[1])
        return data, labels


def read_text(filename):
    data = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header
        for row in reader:
            text, *_ = row
            data.append(text)
    return data


def preprocess_data(data):
    # stop_words = set(stopwords.words('english'))
    return [' '.join(words).lower() for words in data]
    # return [' '.join([word.lower() for word in words if word not in stop_words]) for words in data]


def vectorize_data(data, vocabulary):
    vectors = []
    for text in data:
        vector = [text.count(word) for word in vocabulary]
        vectors.append(vector)
    return vectors


def create_vocabulary(data, min_freq=1):
    counter = Counter()
    for text in data:
        counter.update(text.split())
    return [word for word, freq in counter.items() if freq >= min_freq]


def main():
    # File
    train_file = 'data/train_set.csv'
    validation_file = 'data/validation_set.csv'
    test_file = 'data/classification_test_set.csv'
    result_file = 'data/21307035_dengxuying_KNN_classification.csv'

    # Train
    train_data, train_labels = read_data(train_file)
    preprocess_train_data = preprocess_data(train_data)
    vocabulary = create_vocabulary(preprocess_train_data)
    train_vectors = vectorize_data(preprocess_train_data, vocabulary)

    # # plot_accuracy
    # k_set=[]
    # accuracy_set=[]
    # validation_data, validation_labels = read_data(validation_file)
    # preprocess_validation_data = preprocess_data(validation_data)
    # validation_vectors = vectorize_data(preprocess_validation_data, vocabulary)
    # for k in range(40,60):
    #     classifier = KNNClassifier(k)
    #     classifier.fit(train_vectors, train_labels)
    #     validation_predictions = classifier.predict(validation_vectors)
    #     accuracy = sum([1 for pred, true in zip(validation_predictions, validation_labels) if pred == true]) / len(
    #        validation_labels)
    #     k_set.append(k)
    #     accuracy_set.append(accuracy)
    # plt.plot(k_set,accuracy_set)
    # plt.title('Jaccard distance')
    # plt.xlabel('k')
    # plt.ylabel('accuracy')
    # plt.grid()
    # plt.show()

    k = 46
    classifier = KNNClassifier(k)
    classifier.fit(train_vectors, train_labels)

    # Validate
    validation_data, validation_labels = read_data(validation_file)
    preprocess_validation_data = preprocess_data(validation_data)
    validation_vectors = vectorize_data(preprocess_validation_data, vocabulary)
    validation_predictions = classifier.predict(validation_vectors)
    accuracy = sum([1 for pred, true in zip(validation_predictions, validation_labels) if pred == true]) / len(
        validation_labels)
    print(f'Validation accuracy: {accuracy}')

    # Test
    test_data, test_labels = read_data(test_file)
    preprocess_test_data = preprocess_data(test_data)
    test_vectors = vectorize_data(preprocess_test_data, vocabulary)
    text = read_text(test_file)
    test_predictions = classifier.predict(test_vectors)
    test_accuracy = sum([1 for pred, true in zip(test_predictions, test_labels)if pred == true]) / len(test_labels)
    print(f'Test accuracy: {test_accuracy}')
    # Write answers
    with open(result_file, 'w', newline='') as file:
        writer = csv.writer(file)
        flag = 0
        writer.writerow(['Words (split by space)', 'label'])
        for predicted_label in test_predictions:
            writer.writerow([text[flag], predicted_label])
            flag = flag + 1


if __name__ == '__main__':
    start_time = process_time()
    main()
    end_time = process_time()
    print(f'processing time: {end_time - start_time} seconds')
