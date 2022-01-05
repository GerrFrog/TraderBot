# **Metrics**
### A metric is a function that is used to judge the performance of your model.<br><br>Metric functions are similar to loss functions, except that the results from evaluating a metric are not used when training the model. Note that you may use any loss function as a metric.

- ## Accuracy metrics
Calculates how often predictions equal labels.
```python
model.compile(optimizer='sgd',
              loss='mse',
              metrics=[tf.keras.metrics.Accuracy()])
```

- ## Binary Accuracy metrics
Calculates how often predictions match binary labels.<br>

This metric creates two local variables, total and count that are used to compute the frequency with which y_pred matches y_true. This frequency is ultimately returned as binary accuracy: an idempotent operation that simply divides total by count.
```python
model.compile(optimizer='sgd',
              loss='mse',
              metrics=[tf.keras.metrics.BinaryAccuracy()])
```

- ## Categorical Accuracy metrics
Calculates how often predictions match one-hot labels.<br>
This metric creates two local variables, total and count that are used to compute the frequency with which y_pred matches y_true. This frequency is ultimately returned as categorical accuracy: an idempotent operation that simply divides total by count.
```python
model.compile(
  optimizer='sgd',
  loss='mse',
  metrics=[tf.keras.metrics.CategoricalAccuracy()])
```

- ## Sparse Categorical Accuracy metrics
Calculates how often predictions match integer labels.<br>
This metric creates two local variables, total and count that are used to compute the frequency with which y_pred matches y_true. This frequency is ultimately returned as sparse categorical accuracy: an idempotent operation that simply divides total by count.
```python
model.compile(
    optimizer='sgd',
    loss='mse',
    metrics=[tf.keras.metrics.SparseCategoricalAccuracy()])
```

- ## Top KCategorical Accuracy metrics
Computes how often targets are in the top K predictions.
```python
model.compile(optimizer='sgd',
              loss='mse',
              metrics=[tf.keras.metrics.TopKCategoricalAccuracy()])
```

- ## Sparse Top KCategorical Accuracy metrics
Computes how often integer targets are in the top K predictions.
```python
model.compile(
  optimizer='sgd',
  loss='mse',
  metrics=[tf.keras.metrics.SparseTopKCategoricalAccuracy()])
```

