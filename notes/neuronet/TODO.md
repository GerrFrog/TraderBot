#### TODO:
- [x] `Next candle price predict` - predict next candle close value (regression)
- [ ] `Next 2 candles price predict` - predict next 2 close value (output 2 Dense units)
- [x] `Use special columns` - use several columns to predict next close value
- [x] `Curve` - predict curve of graphic in future days
- [ ] `Buy/Sell` - predict Buy/Sell moments (binary classification)
- [ ] `News analysis` - use NLP, BERT models to analyse market

<img src="assets/neuronet.png"></img>

## Architecture:
- [x] LSTM
- [x] BiLSMT
- [x] Conv + MaxPolling + LSTM
- [x] Conv + MaxPolling + BiLSTM
- [x] Timedistributed
- [ ] Attention (e.g. Seq2Seq)
- [ ] Conv and Pooling make curve smoother and LSTM models predict N_STEP day. 1st model predict 1st day, 2nd model predict 2nd day and etc. (build a graph) 
- [ ] After predicting a value, when we want to predict the next value, the data of the previous 60 days (which is used to predict this “next value”) should include our predicted value, and this article uses all the real data to make the prediction instead of the predicted data.

## Check
- [helstm](https://www.google.com/search?client=firefox-b-e&q=helstm+keras)
- [custom layers lstm](https://www.google.com/search?client=firefox-b-e&q=keras+making+custom+layer)