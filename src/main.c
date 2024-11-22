#include <stdio.h>
#include <unistd.h> 
#include "weather.h"


// Main function
int main() {
    // Start the weather monitoring for Karachi 
    while (1) {
        fetchWeatherData("karachi"); // Fetch weather data for Karachi
        sleep(10); //after an hour
    }
   
    return 0;
}
