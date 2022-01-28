#!/bin/bash

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 1d
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 12h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 8h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 6h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 4h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 2h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 1h
echo "[+] Sleeping for 60 seconds..."
sleep 60

python3 scripts/download_data.py --symbol BTCUSDT --start 2010 --end 2022 --timeframe 30m
echo "[+] Sleeping for 180 seconds..."
sleep 180

python3 scripts/download_data.py --symbol BTCUSDT --start 2020 --end 2022 --timeframe 15m
echo "[+] Sleeping for 180 seconds..."
sleep 180

python3 scripts/download_data.py --symbol BTCUSDT --start 2020 --end 2022 --timeframe 5m
echo "[+] Sleeping for 180 seconds..."
sleep 180

python3 scripts/download_data.py --symbol BTCUSDT --start 2020 --end 2022 --timeframe 1m
