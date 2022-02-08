# How Configuration file of Indicators looks like
## **Build-in Indicators (Indicators::Integral)**
### Exponential Moving Average (EMA)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Weighted Moving Average (WMA)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Simple Moving Average (SMA)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Smoothed Moving Average (SSMA)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### Relative Strength Index (RSI)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

### True Range (TR)
```JSON
{
    "period": 11,
    "interval": "1d"
}
```
- **period:** Period.
- **interval:** Interval (candle timeframe).

**NOTE:** Actually this parameters are not needed to calculate TR buy they are needed for programm

### Average True Range (ATR)
```JSON
{
    "period": 11,
    "interval": "1d",
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
    "interval": "1d",
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
    "interval": "1d",
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
    "interval": "1d",
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











