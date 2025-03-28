## README

运行时，注意修改文件的路径

- `NaiveBayesClassifier.py`

  使用朴素贝叶斯实现文本分类

  运行前，根据实际情况修改`main`函数中`alpha`的取值，默认`alpha=1e-3`

  运行方法：在终端运行

  ```shell
  python NaiveBayesClassifier.py
  ```

- `KNNClassifier.py`

​      使用KNN实现文本分类

​      运行前，根据实际情况修改`KNNClassifier`类`predict`函数中的距离公式

```python
distances = [(i, self.euclidean_distance(x, train_x)) for i, (train_x, _) in enumerate(self.train_data)]
```

​      距离公式包括：欧氏距离`euclidean_distance`、余弦距离`cosine_distance`和Jaccard距离     `jaccard_distance`三种，默认为欧氏距离

​     训练模型时的K值需要根据实际情况修改，默认`k=46`

​      运行方法：在终端运行

```shell
python KNNClassifier.py
```

