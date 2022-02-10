# Worker
How basic Worker configuration file looks like
```json
{
    "strategy_name": {
        "1": {
            "trader_params": {
                ...
            },
            "strategy_params": {
                ...
            }
        },
        "2": {
            ...
        }
        ...
    }
}
```
- **strategy_name:** class name of strategy the worker uses.
- **{number}:** an identificational number of worker.
    - **trader_params:** parameters for trader.
    - **strategy_params:** paramters for strategy.

Example:
```json
{
    "RSXC_ADX": {
        "1": {
            "trader_params": {
                "exchange": "binance",
                "symbol": "BTC/USDT",
                "name": "RSXC_LB and ADX signals #1",
                "interval": "5m",
                "candle": "Candle",
                "strategy": {
                    "type": "position",
                    "stake_amount": "all",
                    "symbol_amount": "all",
                    "trade": "both",
                    "short_goal": "symbol",
                    "max_open_trade": 10,
                    "stop-loss": 0.2,
                    "target": 0.15
                }
            },
            "strategy_params": {
                "rsxc_lb": {
                    "type": "Indicators::TradingView",
                    "indicator": "RSXC_LB",
                    "candle": "Candle",
                    "indicator_params": {
                        "interval": "5m",
                        "src": "Close",
                        "length": 14
                    }
                },
                "adx": {
                    "type": "Indicators::Integral",
                    "indicator": "ADX",
                    "candle": "Candle",
                    "indicator_params": {
                        "interval": "5m",
                        "period": 14,
                        "dmi_period": 14,
                        "dmi_smoothed": "RMA",
                        "dmi_atr_smoothed": "RMA"
                    }
                }
            }
        }
    }
}
```


