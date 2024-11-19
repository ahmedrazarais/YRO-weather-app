#!/bin/bash

# Navigate to the project root directory
cd "/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/"

# Compile the C program (ensure this path is correct for your environment)
gcc src/main.c src/weather.c -o weather_app -lcurl -lcjson

# Run the program (ensure it's compiled correctly)
./weather_app
