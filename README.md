# YRO Weather App

A CLI-based weather app fetching real-time data from OpenWeather API using `curl` and parsing it with `cJSON`. The app issues alerts for critical conditions using Linux system calls and logs events using `syslog`.

## Features
- Real-time weather data retrieval and display.
- JSON data parsing for key weather details.
- Real-time alerts for high temperature, humidity, and wind speed.
- Data storage in raw and processed formats.


## Project Structure

/YRO-weather-app
    ├── /data
    │   ├── raw_data.txt       # Raw weather data (appended every API call)
    │   └── processed_data.txt # Processed averages of weather data
    ├── /scripts
    │   └── retrieve_data.sh   # Bash script to compile and run the app
    ├── /src
    │   ├── main.c             # Main program file
    │   ├── weather.c          # Weather data fetching and processing
    │   └── weather.h          # Header file containing structure definitions


## Prerequisites
To run this application, you need to have the following installed:
- **C Compiler (GCC)**
- **libcurl** and **cJSON**
- **Linux Environment** (for alerts and syslog)

### Setup on Ubuntu
1. **Update your system:**
    ```bash
    sudo apt-get update
    ```

2. **Install necessary dependencies:**
    ```bash
    sudo apt-get install gcc libcurl4-openssl-dev libcjson-dev
    ```

3. **Clone the repository:**
    ```bash
    git clone https://github.com/ahmedrazarais/YRO-Weather-App.git
    ```

4. **Navigate to the project directory:**
    ```bash
    cd YRO-weather-app
    ```

### Build & Run
1. **Compile the program using the provided script:**
    ```bash
    cd scripts
    ./retrieve_data.sh
    ```

    Alternatively, you can compile manually with:
    ```bash
    cd src/
    gcc -o weather_app src/main.c src/weather.c -lcurl -lcjson
    ```

2. **Run the program:**
    ```bash
    ./weather_app
    ```

### Alerts
- **High Temperature Alert:** Triggered if the average temperature is greater than 35°C.
- **High Humidity Alert:** Triggered if the average humidity is greater than 80%.
- **High Wind Speed Alert:** Triggered if the average wind speed is greater than 15 m/s.

**Note:** Make sure to replace the API key in `weather.c` with your own key from OpenWeather API.

### Background Execution

You can run the application in the background using `nohup`:

```bash
nohup ./weather_app &

```
- To kill the running background process:

- First, get the process ID (PID):
```bash
ps aux | grep weather_app
```

- Then, kill the process using its PID:

```bash
kill <PID>
```

# Script File
- The retrieve_data.sh script automates the compilation and execution of the program. Simply run the script as follows:

```bash
cd scripts
./retrieve_data.sh

```





# For run in background
nohup ./weather_app &

#kill from background
kill 1582



# Run script file
cd scripts

./retrieve_data.sh
