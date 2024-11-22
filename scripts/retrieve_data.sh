#!/bin/bash

# My root directory path 
cd "/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/"

# Compile the C program command
gcc src/main.c src/weather.c -o weather_app -lcurl -lcjson

# executable path
./weather_app

