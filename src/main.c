#include <stdio.h>
#include <unistd.h> // Include this header for sleep function
#include "weather.h"

// Main function
int main() {
    // Start the weather monitoring for Karachi
    while (1) {
        fetchWeatherData("Boston"); // Fetch weather data for Karachi
        sleep(10); // Fetch every 30 seconds during testing
    }
    return 0;
}
