# Strategy
How basic strategy configuration file looks like:
```json
{
    "indicator_name_1": {
        ...indicator_params...
    },
    "indicator_name_2": {
        ...indicator_params...
    }
}
```
- **indicator_name:** The Strategy would expect this name when gets parameters to be resolved.

For example, we've got Strategy "resolve" function. As you can see Strategy expects Indicator resolved value with name "rsxc_lb" and "adx". You can choose any name for "indicator_name" buy it is recommended to call it by indicator name.
```C
void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)           
{
    signals["buy"] = false;
    signals["sell"] = false;

    if (
        params["rsxc_lb"]["value"] <= this->rsxc_lb_low_line &&
        params["adx"]["value"] >= this->adx_line
    ) 
        signals["buy"] = true;
    if (
        params["rsxc_lb"]["value"] >= this->rsxc_lb_high_line
    )
        signals["sell"] = true;
}
```
And configuration file must look like:
```json
{
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
```

# Released Strategies
- [x] EMA Cross
- [x] Normalized MACD Cross
- [x] RSXC-ADX Strategy

# Create new Strategy
1. Specify what indicators strategy will use and their names (indicator_name).
2. Describe the strategy in src/strategies




