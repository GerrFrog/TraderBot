# Indicators
How basic Indicator configuration file looks like:
```json
{
    "type": "Indicators::TradingView",
    "indicator": "RSXC_LB",
    "candle": "Candle",
    "indicator_params": {
        "interval": "5m",
        ...
}
```
- **type:** from where to take indicator
- **indicator:** class name of indicator
- **candle:** which candle type indicator uses to calculate itself
- **indicator_params:** parameters for indicator
    - **interval:** Interval for Indicator (candle timeframe).<br>**NOTE:** Interval must be in each indicator configuration

After "interval" you can specify which parameters Indicator uses to calculate itself. You can find Indicator parameters futher down the page. For example, Exponential Moving Average (EMA) gets "src" (source from candle) and "period" (period of EMA).
```json
{
    "type": "Indicators::Integral",
    "indicator": "EMA",
    "candle": "Candle",
    "indicator_params": {
        "interval": "5m",
        "src": "Close",
        "period": 14
}
```

# Create new Indicator
1. Specify how config for strategy with this indicator would look like in config.json
2. Describe the indicator in src/indicators
3. Add indicator to watcher initialization in src/workers
    - **initialize():** add indicator to watcher inititalize function
    - **get():** add indicator to get function
    - **resolve()** add this indicator to resolving

# Released Indicators
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
    - [x] Relative Strength Index (RSI)
    - [x] Simple Moving Average (SMA)
    - [x] Weighted Moving Average (WMA)
- **Indicators::TradingView**<br>
    Overwritten pine-scripts to C++. Object calculates value of technical indicator by passed candle.<br>
    Implemented technical indicators:
    - [x] Normalized Moving Average Convergence/Divergence by "glaz" (Normalize_MACD)

# How "indicator_params" for Indicators looks like
## **Build-in Indicators (Indicators::Integral)**
### Exponential Moving Average (EMA)
```JSON
{
    "src": "Close",
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Weighted Moving Average (WMA)
```JSON
{
    "src": "Close",
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Simple Moving Average (SMA)
```JSON
{
    "src": "Close",
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Smoothed Moving Average (SSMA)
```JSON
{
    "src": "Close",
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Relative Strength Index (RSI)
```JSON
{
    "src": "Close",
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### True Range (TR)
```JSON
{
    "period": 11
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

**NOTE:** Actually this parameters are not needed to calculate TR buy they are needed for programm

### Average True Range (ATR)
```JSON
{
    "period": 11,
    "smoothed": "RMA"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).
- **smoothed:** Smoothing function (RMA, WMA, SMA, EMA). For default, use RMA.

### Directional Movement Index (DMI)
```JSON
{
    "period": 14,
    "smoothed": "RMA",
    "atr_period": 14,
    "atr_smoothed": "RMA"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).
- **smoothed:** Smoothing function (RMA, Smooth, EMA). For default, use RMA.
- **atr_period:** Period for ATR indicator (typically as DMI period).
- **atr_smoothed:** Smoothing function for ATR indicator (typically use RMA).

### Average Directional Index (ADX)
```JSON
{
    "period": 14,
    "dmi_period": 14,
    "dmi_smoothed": "RMA",
    "dmi_atr_smoothed": "RMA"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).
- **dmi_period:** Period for DMI indicator (typically as ADX period)
- **dmi_smoothed:** Smoothing function for DMI indicator (typically RMA)
- **dmi_atr_smoothed:** Smoothing function for ATR indicator in DMI indicator usage (typically RMA)

## **Custom TradingView indicators (Indicators::TradingView)**
### Normalized MACD (Normalized_MACD)
```JSON
{
    "src": "Close",
    "type": "EMA",
    "sma": 12,
    "lma": 21,
    "tsp": 9,
    "np": 50
}
```
- **interval:** Interval (candle timeframe).
- **type:** Type of Moving Average (EMA, WMA, SMA). Typically EMA
- **sma:** Period of Fast Moving Average
- **lma:** Period of Slow Moving Average
- **tsp:** Period of Trigger
- **np:** Period of MacNorm
### JMA RSX Clone (RSXC_LB)
```JSON
{
    "src": "Close",
    "length": 14,
}
```
- **interval:** Interval (candle timeframe).
- **length:** Period.
- **src:** Source from candle (Open, Close, Low, High).











