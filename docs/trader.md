# Trader
There are 2 types of configuration a Trader:
- **scalping:** This configuration requires "stop-loss" and "target" in percentage. If candle lossed "stop-loss" percent of price the trade will be closed as lose. If candle reached "target" percent of price the trade will be closed as win.
- **position**: This configuration doesn't require anything. Trader opens "long" position when it is "buy" signal and closes trade when it is "sell" position. Trader opens "short" position when it is "sell" signal and closes trade when it is "buy" position.

To open a trade you should also determine "stake_amount" and "symbol_amount". "stake_amount" is a count of USDT you will buy a cryptocurrency to open a long trade. "symbol_amount" is a count of symbol you will sell a cryptocurrency to open a short trade.


How basic configuration Trader file looks like:
```json
{
    "exchange": "binance",
    "symbol": "BTC/USDT",
    "name": "RSXC_LB and ADX signals #1",
    "interval": "5m",
    "candle": "Candle",
    "strategy": {
        "type": "position",
        "positions": "both",
        "stake_amount": "all",
        "symbol_amount": "all",
        "short_goal": "symbol",
        "max_open_trade": 10,
        "stop-loss": 0.2,
        "target": 0.15
    }
}
```
- **exchange:** where to trade.
- **symbol:** what to trade.
- **name:**  trader name.
- **interval:** interval (candle timeframe) the trader trades.
- **candle:** type of candle trader uses.
- **strategy:** parameters of trader strategy.
    - **type:** trading type.
    - **positions:** "short", "long" or "both" trades are used.
    - **stake_amount**
        - **{number}:** buy {symbol} for {number} USDT.
        - **all:** use all balance to enter in trade.
    - **symbol_amount**
        - **{number}:** sell {number} of {symbol}. After it has reached target (or buy signal) bot buys the same {symbol} count as {number}.
        - **all:** use all balance to enter in trade.
    - **short_goal:** what short trades earn. If "symbol" is bot sell crypto and buy as many as possible (it increases "symbol" balance). If "usd" is bot sell crypto and buy as much as sold. The difference of USDT between "sell" and "buy" is your profit (it increases "usd" balance).
    - **max_open_trade:** maximum opened trades.
    - **stop-loss:** stop-loss point (in percents) (e.g. 0.2 = 0.2%). Needed only when "scalping" trading type.
    - **target:** target point (in percents) (e.g. 0.15 = 0.15%). Needed onlye when "scalping" trading type.

**NOTE:** If {"stake_amount" = -1} then it uses {balance / "max_open_trade"} to open a long trade.

**NOTE:** If {"symbol_amount" = -1} then it uses {balance / "max_open_trade"} to open a short trade.

**NOTE:** If {"max_open_trade" = -1} then it opens unlimited number of trades.

**NOTE** Trader name must be unique

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