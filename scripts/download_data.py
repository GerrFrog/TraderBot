from binance.client import Client
from binance.enums import *
from dotenv import load_dotenv

import os
import argparse

import pandas as pd
import numpy as np

load_dotenv()

private_key = os.getenv("PRIVATE_KEY")
secret_key = os.getenv("SECRET_KEY")

if not private_key:
    load_dotenv("../.env")

    private_key = os.getenv("PRIVATE_KEY")
    secret_key = os.getenv("SECRET_KEY")

client = Client(private_key, secret_key)
parser = argparse.ArgumentParser()

parser.add_argument(
    "--symbol",
    help="Pair to download data",
    required=True
)
parser.add_argument(
    "--timeframe",
    help="Timeframe for candles",
    required=True
)
parser.add_argument(
    "--start",
    help="Start date to download",
    required=True
)
parser.add_argument(
    "--end",
    help="End date to download",
    required=True
)

args = vars(parser.parse_args())

timeframe = args["timeframe"]
symbol = args["symbol"]
start = args["start"]
end = args["end"]

print("[+] Starting download data for {} with {} timeframe from {} to {}".format(symbol, timeframe, start, end))

candles = client.get_historical_klines(
    symbol=symbol,
    interval=timeframe,
    start_str=start,
    end_str=end
)

candles = np.array(candles)

df = pd.DataFrame(
    candles.reshape(-1, 12), 
    dtype=float, 
    columns=(
        'Open Time',
        'Open',
        'High',
        'Low',
        'Close',
        'Volume',
        'Close time',
        'Quote asset volume',
        'Number of trades',
        'Taker buy base asset volume',
        'Taker buy quote asset volume',
        'Ignore'
    )
)

try:
    df.to_csv("../data/{}_{}.csv".format(symbol, timeframe), sep='\t')
except OSError:
    df.to_csv("./data/{}_{}.csv".format(symbol, timeframe), sep='\t')
except Exception as exp:
    print("Something went wrong!")    
    print(exp)












