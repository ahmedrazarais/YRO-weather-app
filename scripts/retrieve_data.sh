#!/bin/bash

# Navigate to the project root directory
cd "/mnt/c/Users/ideal pc/Desktop/cew"

# Compile the C program
gcc src/main.c src/weather.c -o weather -lcurl -lcjson

# Run the program
./weather_app

