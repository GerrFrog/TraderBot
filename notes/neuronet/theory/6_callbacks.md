# Callbacks
A callback is an object that can perform actions at various stages of training (e.g. at the start or end of an epoch, before or after a single batch, etc).<br><br>
**You can use callbacks to:**
- Write TensorBoard logs after every batch of training to monitor your metrics
- Periodically save your model to disk
- Do early stopping
- Get a view on internal states and statistics of a model during training
- ...and more

### Example:
```python
# The relevant methods of the callbacks will then be called at each stage of the training.
my_callbacks = [
    tf.keras.callbacks.EarlyStopping(patience=2),
    tf.keras.callbacks.ModelCheckpoint(filepath='model.{epoch:02d}-{val_loss:.2f}.h5'),
    tf.keras.callbacks.TensorBoard(log_dir='./logs'),
]

model.fit(dataset, epochs=10, callbacks=my_callbacks)
```

## **New custom callback**
```python
# https://keras.io/guides/writing_your_own_callbacks
training_finished = False
class MyCallback(tf.keras.callbacks.Callback):
  def on_train_end(self, logs=None):
    global training_finished
    training_finished = True
model = tf.keras.Sequential([tf.keras.layers.Dense(1, input_shape=(1,))])
model.compile(loss='mean_squared_error')
model.fit(tf.constant([[1.0]]), tf.constant([[1.0]]),
          callbacks=[MyCallback()])
assert training_finished == True
```

## **Model Checkpoint**
Callback to save the Keras model or model weights at some frequency.<br><br>
ModelCheckpoint callback is used in conjunction with training using model.fit() to save a model or weights (in a checkpoint file) at some interval, so the model or weights can be loaded later to continue the training from the state saved.<br><br>
A few options this callback provides include:
- Whether to only keep the model that has achieved the "best performance" so far, or whether to save the model at the end of every epoch regardless of performance.
- Definition of 'best'; which quantity to monitor and whether it should be maximized or minimized.
- The frequency it should save at. Currently, the callback supports saving at the end of every epoch, or after a fixed number of training batches.
Whether only weights are saved, or the whole model is saved.
```python
model.compile(loss=..., optimizer=...,
              metrics=['accuracy'])

EPOCHS = 10
checkpoint_filepath = '/tmp/checkpoint'
model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
    filepath=checkpoint_filepath,
    save_weights_only=True,
    monitor='val_accuracy',
    mode='max',
    save_best_only=True)

# Model weights are saved at the end of every epoch, if it's the best seen
# so far.
model.fit(epochs=EPOCHS, callbacks=[model_checkpoint_callback])

# The model weights (that are considered the best) are loaded into the model.
model.load_weights(checkpoint_filepath)
```
- **filepath:** string or PathLike, path to save the model file. e.g. filepath = os.path.join(working_dir, 'ckpt', file_name). filepath can contain named formatting options, which will be filled the value of epoch and keys in logs (passed in on_epoch_end). For example: if filepath is weights.{epoch:02d}-{val_loss:.2f}.hdf5, then the model checkpoints will be saved with the epoch number and the validation loss in the filename. The directory of the filepath should not be reused by any other callbacks to avoid conflicts.
- **monitor:** The metric name to monitor. Typically the metrics are set by the Model.compile method. Note:
    - Prefix the name with "val_" to monitor validation metrics.
    - Use "loss" or "val_loss" to monitor the model's total loss.
    - If you specify metrics as strings, like "accuracy", pass the same string (with or without the "val_" prefix).
    - If you pass metrics.Metric objects, monitor should be set to metric.name
    - If you're not sure about the metric names you can check the contents of the history.history dictionary returned by history = model.fit()
    - Multi-output models set additional prefixes on the metric names.
    - **verbose:** verbosity mode, 0 or 1.
    - **save_best_only:** if save_best_only=True, it only saves when the model is considered the "best" and the latest best model according to the quantity monitored will not be overwritten. If filepath doesn't contain formatting options like {epoch} then filepath will be overwritten by each new better model.
    - **mode:** one of {'auto', 'min', 'max'}. If save_best_only=True, the decision to overwrite the current save file is made based on either the maximization or the minimization of the monitored quantity. For val_acc, this should be max, for val_loss this should be min, etc. In auto mode, the mode is set to max if the quantities monitored are 'acc' or start with 'fmeasure' and are set to min for the rest of the quantities.
    - **save_weights_only:** if True, then only the model's weights will be saved (model.save_weights(filepath)), else the full model is saved (model.save(filepath)).
    - **save_freq:** 'epoch' or integer. When using 'epoch', the callback saves the model after each epoch. When using integer, the callback saves the model at end of this many batches. If the Model is compiled with steps_per_execution=N, then the saving criteria will be checked every Nth batch. Note that if the saving isn't aligned to epochs, the monitored metric may potentially be less reliable (it could reflect as little as 1 batch, since the metrics get reset every epoch). Defaults to 'epoch'.
    - **options:** Optional tf.train.CheckpointOptions object if save_weights_only is true or optional tf.saved_model.SaveOptions object if save_weights_only is false.
    - ****kwargs:** Additional arguments for backwards compatibility. Possible key is period.

## **Tensorboard**
Enable visualizations for TensorBoard.
```python
tf.keras.callbacks.TensorBoard(
    log_dir="logs",
    histogram_freq=0,
    write_graph=True,
    write_images=False,
    write_steps_per_second=False,
    update_freq="epoch",
    profile_batch=2,
    embeddings_freq=0,
    embeddings_metadata=None,
    **kwargs
)
```
- **log_dir:** the path of the directory where to save the log files to be parsed by TensorBoard. e.g. log_dir = os.path.join(working_dir, 'logs') This directory should not be reused by any other callbacks.
- **histogram_freq:** frequency (in epochs) at which to compute activation and weight histograms for the layers of the model. If set to 0, histograms won't be computed. Validation data (or split) must be specified for histogram visualizations.
- **write_graph:** whether to visualize the graph in TensorBoard. The log file can become quite large when write_graph is set to True.
- **write_images:** whether to write model weights to visualize as image in TensorBoard.
- **write_steps_per_second:** whether to log the training steps per second into Tensorboard. This supports both epoch and batch frequency logging.
- **update_freq:** 'batch' or 'epoch' or integer. When using 'batch', writes the losses and metrics to TensorBoard after each batch. The same applies for 'epoch'. If using an integer, let's say 1000, the callback will write the metrics and losses to TensorBoard every 1000 batches. Note that writing too frequently to TensorBoard can slow down your training.
- **profile_batch:** Profile the batch(es) to sample compute characteristics. profile_batch must be a non-negative integer or a tuple of integers. A pair of positive integers signify a range of batches to profile. By default, it will profile the second batch. Set profile_batch=0 to disable profiling.
- **embeddings_freq:** frequency (in epochs) at which embedding layers will be visualized. If set to 0, embeddings won't be visualized.
- **embeddings_metadata:** Dictionary which maps embedding layer names to the filename of a file in which to save metadata for the embedding layer. In case the same metadata file is to be used for all embedding layers, a single filename can be passed.
```shell
$ tensorboard --logdir=path_to_your_logs
```
### Example
```python
tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir="./logs")
model.fit(x_train, y_train, epochs=2, callbacks=[tensorboard_callback])
```

## **Early Stopping**
Stop training when a monitored metric has stopped improving.<br><br>
Assuming the goal of a training is to minimize the loss. With this, the metric to be monitored would be 'loss', and mode would be 'min'. A model.fit() training loop will check at end of every epoch whether the loss is no longer decreasing, considering the min_delta and patience if applicable. Once it's found no longer decreasing, model.stop_training is marked True and the training terminates.
```python
tf.keras.callbacks.EarlyStopping(
    monitor="val_loss",
    min_delta=0,
    patience=0,
    verbose=0,
    mode="auto",
    baseline=None,
    restore_best_weights=False,
)
```
- **monitor:** Quantity to be monitored.
- **min_delta:** Minimum change in the monitored quantity to qualify as an improvement, i.e. an absolute change of less than min_delta, will count as no improvement.
- **patience:** Number of epochs with no improvement after which training will be stopped.
- **verbose:** verbosity mode.
- **mode:** One of {"auto", "min", "max"}. In min mode, training will stop when the quantity monitored has stopped decreasing; in "max" mode it will stop when the quantity monitored has stopped increasing; in "auto" mode, the direction is automatically inferred from the name of the monitored quantity.
- **baseline:** Baseline value for the monitored quantity. Training will stop if the model doesn't show improvement over the baseline.
- **restore_best_weights:** Whether to restore model weights from the epoch with the best value of the monitored quantity. If False, the model weights obtained at the last step of training are used. An epoch will be restored regardless of the performance relative to the baseline. If no epoch improves on baseline, training will run for patience epochs and restore weights from the best epoch in that set.

### Example
```python
callback = tf.keras.callbacks.EarlyStopping(monitor='loss', patience=3)
# This callback will stop the training when there is no improvement in
# the loss for three consecutive epochs.
model = tf.keras.models.Sequential([tf.keras.layers.Dense(10)])
model.compile(tf.keras.optimizers.SGD(), loss='mse')
history = model.fit(np.arange(100).reshape(5, 20), np.zeros(5),
                    epochs=10, batch_size=1, callbacks=[callback],
                    verbose=0)
print(len(history.history['loss']))  # Only 4 epochs are run.
```

## **Learning Rate  Scheduler**
Learning rate scheduler.<br><br>
At the beginning of every epoch, this callback gets the updated learning rate value from schedule function provided at __init__, with the current epoch and current learning rate, and applies the updated learning rate on the optimizer.
```python
tf.keras.callbacks.LearningRateScheduler(schedule, verbose=0)
```
- **schedule:** a function that takes an epoch index (integer, indexed from 0) and current learning rate (float) as inputs and returns a new learning rate as output (float).
- **verbose:** int. 0: quiet, 1: update messages.

### Example
```python
# This function keeps the initial learning rate for the first ten epochs
# and decreases it exponentially after that.
def scheduler(epoch, lr):
  if epoch < 10:
    return lr
  else:
    return lr * tf.math.exp(-0.1)

model = tf.keras.models.Sequential([tf.keras.layers.Dense(10)])
model.compile(tf.keras.optimizers.SGD(), loss='mse')
print(round(model.optimizer.lr.numpy(), 5)) # 0.01

callback = tf.keras.callbacks.LearningRateScheduler(scheduler)

history = model.fit(np.arange(100).reshape(5, 20), np.zeros(5),
                    epochs=15, callbacks=[callback], verbose=0)
print(round(model.optimizer.lr.numpy(), 5)) # 0.00607
```

## **Reduce LR On Plateau**
Reduce learning rate when a metric has stopped improving.<br><br>
Models often benefit from reducing the learning rate by a factor of 2-10 once learning stagnates. This callback monitors a quantity and if no improvement is seen for a 'patience' number of epochs, the learning rate is reduced.
```python
tf.keras.callbacks.ReduceLROnPlateau(
    monitor="val_loss",
    factor=0.1,
    patience=10,
    verbose=0,
    mode="auto",
    min_delta=0.0001,
    cooldown=0,
    min_lr=0,
    **kwargs
)
```
- **monitor:** quantity to be monitored.
- **factor:** factor by which the learning rate will be reduced. new_lr = lr * factor.
- **patience:** number of epochs with no improvement after which learning rate will be reduced.
- **verbose:** int. 0: quiet, 1: update messages.
- **mode:** one of {'auto', 'min', 'max'}. In 'min' mode, the learning rate will be reduced when the quantity monitored has stopped decreasing; in 'max' mode it will be reduced when the quantity monitored has stopped increasing; in 'auto' mode, the direction is automatically inferred from the name of the monitored quantity.
- **min_delta:** threshold for measuring the new optimum, to only focus on significant changes.
- **cooldown:** number of epochs to wait before resuming normal operation after lr has been reduced.
- **min_lr:** lower bound on the learning rate.

### Example
```python
reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.2,
                              patience=5, min_lr=0.001)
model.fit(X_train, Y_train, callbacks=[reduce_lr])
```

## **Remote Monitor**
Callback used to stream events to a server.<br><br>
Requires the requests library. Events are sent to root + '/publish/epoch/end/' by default. Calls are HTTP POST, with a data argument which is a JSON-encoded dictionary of event data. If send_as_json=True, the content type of the request will be "application/json". Otherwise the serialized JSON will be sent within a form.
```python
tf.keras.callbacks.RemoteMonitor(
    root="http://localhost:9000",
    path="/publish/epoch/end/",
    field="data",
    headers=None,
    send_as_json=False,
)
```
- **root:** String; root url of the target server.
- **path:** String; path relative to root to which the events will be sent.
- **field:** String; JSON field under which the data will be stored. The field is used only if the payload is sent within a form (i.e. send_as_json is set to False).
- **headers:** Dictionary; optional custom HTTP headers.
- **send_as_json:** Boolean; whether the request should be sent as "application/json".

## **CSV Logger**
Callback that streams epoch results to a CSV file.
```python
csv_logger = CSVLogger('training.log')
model.fit(X_train, Y_train, callbacks=[csv_logger])
```

## **Progbar Logger**
Callback that prints metrics to stdout.
```python
tf.keras.callbacks.ProgbarLogger(count_mode="samples", stateful_metrics=None)
```

## **Backup And Restore**
Callback to back up and restore the training state.<br><br>
BackupAndRestore callback is intended to recover from interruptions that happened in the middle of a model.fit execution by backing up the training states in a temporary checkpoint file (based on TF CheckpointManager) at the end of each epoch. If training restarted before completion, the training state and model are restored to the most recently saved state at the beginning of a new model.fit() run. Note that user is responsible to bring jobs back up. This callback is important for the backup and restore mechanism for fault tolerance purpose. And the model to be restored from an previous checkpoint is expected to be the same as the one used to back up. If user changes arguments passed to compile or fit, the checkpoint saved for fault tolerance can become invalid.<br>
Note: 
1. This callback is not compatible with disabling eager execution. 
2. A checkpoint is saved at the end of each epoch, when restoring we'll redo any partial work from an unfinished epoch in which the training got restarted (so the work done before a interruption doesn't affect the final model state). 
3. This works for both single worker and multi-worker mode, only MirroredStrategy and MultiWorkerMirroredStrategy are supported for now.
### Example
```python
class InterruptingCallback(tf.keras.callbacks.Callback):
  def on_epoch_begin(self, epoch, logs=None):
    if epoch == 4:
      raise RuntimeError('Interrupting!')
callback = tf.keras.callbacks.experimental.BackupAndRestore(
backup_dir="/tmp/backup")
model = tf.keras.models.Sequential([tf.keras.layers.Dense(10)])
model.compile(tf.keras.optimizers.SGD(), loss='mse')
try:
  model.fit(np.arange(100).reshape(5, 20), np.zeros(5), epochs=10,
            batch_size=1, callbacks=[callback, InterruptingCallback()],
            verbose=0)
except:
  pass
history = model.fit(np.arange(100).reshape(5, 20), np.zeros(5), epochs=10,
            batch_size=1, callbacks=[callback], verbose=0)
# Only 6 more epochs are run, since first trainning got interrupted at
# zero-indexed epoch 4, second training will continue from 4 to 9.
print(len(history.history['loss'])) # 6
```




















