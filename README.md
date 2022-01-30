# TraderBot
# Create new Indicator
1. Specify how config for strategy with this indicator would look like in config.json
    - **type:** from where to take indicator
    - **indicator:** class name of indicator

Example: for **Indicatos::TAAPI**, **Indicators::Integral**
```json
{
    "type": "Indicators::TAAPI",
    "indicator": "EMA",
    "indicator_params": {
        "period": 50,
        "interval": "1d"
    }
}
```
Example: for **Indicators::TradingView**
```json
{
    "type": "Indicators::TradingView",
    "indicator": "Normalized_MACD",
    "indicator_params": {
        "sma": 12,
        "lma": 21,
        "tsp": 9,
        "np": 50,
        "type": "EMA"
    }
}
```

2. Describe the indicator in src/indicators
3. Add indicator to watcher initialization in src/workers
    - **initialize():** add indicator to watcher inititalize function
    - **get():** add indicator to get function
    - **resolve()** add this indicator to resolving

## **Indicators**
- **Indicators::TAAPI**<br>
    To get a basic value of technical indicator object requests to taapi.io.<br>
    Implemented technical indicators:
    - [x] Exponential Moving Average (EMA)<br>
    - [ ] Relative Strength Index (RSI)
    - [ ] Simple Moving Average (SMA)
    - [ ] Weighted Moving Average (WMA)
- **Indicators::Integral**<br>
    Object calculates value of technical indicator by passed candle.<br>
    Implemented technical indicators:
    - [x] Exponential Moving Average (EMA)
    - [ ] Relative Strength Index (RSI)
    - [x] Simple Moving Average (SMA)
    - [x] Weighted Moving Average (WMA)
- **Indicators::TradingView**<br>
    Overwritten pine-scripts to C++. Object calculates value of technical indicator by passed candle.<br>
    Implemented technical indicators:
    - [x] Normalized Moving Average Convergence/Divergence by "glaz" (Normalize_MACD)

# Create new Strategy
1. Describe new Strategy in src/strategies
2. Add Strategy to Manager in src/managers

# Trader Configuration
There are 2 types of configuration a Trader:
- **scalping:** This configuration requires "stop-loss" and "target" in percentage. If candle lossed "stop-loss" percent of price the trade will be closed as lose. If candle reached "target" percent of price the trade will be closed as win.
- **position**: This configuration doesn't require anything. Trader opens "long" position when it is "buy" signal and closes trade when it is "sell" position. Trader opens "short" position when it is "sell" signal and closes trade when it is "buy" position.

To open a trade you should also determine "stake_amount". It is a count of USDT you will buy a cryptocurrency to open a trade.
- **{number}:** buy {symbol} for {number} USDT.
- **all:** use all balance to enter in trade.

Excample of "scalping" strategy configuration
```json
{
    "exchange": "binance", // where to trade
    "symbol": "BTC/USDT", // what to trade
    "name": "Normalized MACD Cross Strategy #1", // worker name
    "timeframe": "1m", // candle interval to trade
    "strategy": {
        "type": "scalping", // strategy type
        "stake_amount": "all", // stake amount to open trade
        "stop-loss": 0.2, // 0.2%
        "target": 0.15    // 0.15%
}
```
Example of "position" strategy configuration
```json
{
    "exchange": "binance", 
    "symbol": "BTC/USDT", 
    "name": "Normalized MACD Cross Strategy #1", 
    "timeframe": "1m", 
    "strategy": {
        "type": "position", 
        "stake_amount": 100, 
}
```

# Backtesting
Download data
```bash
python3 scripts/download_data.py --timeframe 12h --symbol BTCUSDT --start 2021 --end 2022
```

Download big dataset
```bash
bash scripts/download_big_data.sh
```

**WARNING:** After you have downloaded dataset you should delete last column in csv file to avoid error.

Start backtesting for strategy
```C
Tester tester(
    "../data/",
    taapi_key
);
tester.backtest<Strategies::Normalized_MACD_Cross>(
    jf["strategies"]["normalized_macd_cross"]["1"],
    10000.0
);
```
Start backtesting for indicator
```C
nlohmann::json indicator_params = {
    {"interval", "1d"},
    {"period", 30}
};
Tester tester(
    "../data/",
    taapi_key
);
tester.backtest_indicator<Indicators::Integral::SMA>(
    indicator_params,
    "BTCUSDT",
    "1d"
);
```