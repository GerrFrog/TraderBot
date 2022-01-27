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
    - [ ] Simple Moving Average (SMA)
    - [ ] Weighted Moving Average (WMA)
- **Indicators::TradingView**<br>
    Overwritten pine-scripts to C++. Object calculates value of technical indicator by passed candle.<br>
    Implemented technical indicators:
    - [x] Normalized Moving Average Convergence/Divergence by "glaz" (Normalize_MACD)

# Create new Strategy
1. Describe new Strategy in src/strategies
2. Add Strategy to Manager in src/managers

# Create new Worker and Strategy
1. Describe new Indicator in src/indicators, if it does not exist
2. Describe the Strategy in src/strategies
3. Describe the Worker in src/workers
5. Setup config for Strategy and Trader
6. Add to Manager in src/managers