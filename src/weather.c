#include "weather.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define MAX_API_CALLS 5  // Define maximum number of API calls in a batch

// Forward declaration of the calculateAndSaveAverages function
void calculateAndSaveAverages(void);

// Store the weather data collected in 5 minutes
struct WeatherData rawData[MAX_API_CALLS];
int dataCount = 0;  // To track how many API calls have been made

// Function to initialize string
void init_string(struct string *s) {
    s->ptr = malloc(1);  // initially no memory
    s->len = 0;          // no data at all
    s->ptr[0] = '\0';    // null terminate
}

// Function to handle the response from CURL (write data to string)
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1); // +1 for null terminator
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc failed!\n");
        exit(1);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';  // null terminate
    s->len = new_len;

    return size * nmemb;
}

// Save data to a specified file
void saveDataToFile(const char *data, const char *filename, int isRawData) {
    FILE *file;

    if (isRawData) {
        file = fopen("/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/data/raw_data.txt", "a");  // Append to raw data file
    } else {
        file = fopen("/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/data/processed_data.txt", "a");  // Append to processed data file
    }

    if (file) {
        fprintf(file, "%s\n", data);
        fclose(file);
    } else {
        perror("Failed to open file");
    }
}

// Function to fetch weather data for Karachi
void fetchWeatherData(const char *city) {
    CURL *curl;
    CURLcode res;
    struct string s;
    init_string(&s);

    char url[256];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?appid=877c36bd2208b53b2f0daa5184c1ea03&units=metric&q=%s", city);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            // Display weather data in terminal
            printf("\n------------------------------------------------------\n");
            printf("            Weather Details for %s after 30 sec             \n", city);
            printf("-----------------------------------------------------------\n");

            // Parse the raw data
            cJSON *json = cJSON_Parse(s.ptr);
            if (json) {
                cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
                cJSON *weather = cJSON_GetObjectItemCaseSensitive(json, "weather");
                cJSON *wind = cJSON_GetObjectItemCaseSensitive(json, "wind");

                if (main && weather && wind) {
                    struct WeatherData currentData;
                    currentData.temp = cJSON_GetObjectItemCaseSensitive(main, "temp")->valuedouble;
                    currentData.feels_like = cJSON_GetObjectItemCaseSensitive(main, "feels_like")->valuedouble;
                    currentData.humidity = cJSON_GetObjectItemCaseSensitive(main, "humidity")->valueint;
                    currentData.wind_speed = cJSON_GetObjectItemCaseSensitive(wind, "speed")->valuedouble;

                    // Display the weather data to the terminal
                    printf("Temperature: %.1f°C\n", currentData.temp);
                    printf("Feels Like: %.1f°C\n", currentData.feels_like);
                    printf("Humidity: %d%%\n", currentData.humidity);
                    printf("Wind Speed: %.1f m/s\n", currentData.wind_speed);

                    // Save raw data to file (append data to raw_data.txt)
                    saveDataToFile(s.ptr, "/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/data/raw_data.txt", 1);

                    // Store data in the array
                    rawData[dataCount] = currentData;
                    dataCount++;

                    if (dataCount >= MAX_API_CALLS) {
                        // Once 5 calls are made, calculate averages
                        calculateAndSaveAverages();
                        dataCount = 0;  // Reset for the next batch of data
                        printf("\npass completed and data saved to processed data file\n");
                    }
                }
                cJSON_Delete(json);
            }
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    free(s.ptr);
}

// Function to calculate averages from raw data and save them to processed_data.txt
void calculateAndSaveAverages() {
    double totalTemp = 0;
    double totalFeelsLike = 0;
    int totalHumidity = 0;
    double totalWindSpeed = 0;

    // Accumulate data from rawData
    for (int i = 0; i < MAX_API_CALLS; i++) {
        totalTemp += rawData[i].temp;
        totalFeelsLike += rawData[i].feels_like;
        totalHumidity += rawData[i].humidity;
        totalWindSpeed += rawData[i].wind_speed;
    }

    // Calculate averages
    double avgTemp = totalTemp / MAX_API_CALLS;
    double avgFeelsLike = totalFeelsLike / MAX_API_CALLS;
    double avgHumidity = totalHumidity / MAX_API_CALLS;
    double avgWindSpeed = totalWindSpeed / MAX_API_CALLS;

    // Print the averages to the terminal
    printf("\n\n------------------------------------------------------\n\n");
    printf("             Average Weather Data\n");
    printf("------------------------------------------------------\n");
    printf("Date: %s\n", __DATE__);
    printf("Average Temperature: %.1f°C\n", avgTemp);
    printf("Average Feels Like: %.1f°C\n", avgFeelsLike);
    printf("Average Humidity: %.1f%%\n", avgHumidity);
    printf("Average Wind Speed: %.1f m/s\n", avgWindSpeed);
    printf("------------------------------------------------------\n");

    // Prepare the result string to be saved
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char date[26];
    strftime(date, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    char result[1024];
    snprintf(result, sizeof(result), "Date: %s\nAverage Temperature: %.1f°C\nAverage Feels Like: %.1f°C\nAverage Humidity: %.1f%%\nAverage Wind Speed: %.1f m/s\n---------------------------------------------", date, avgTemp, avgFeelsLike, avgHumidity, avgWindSpeed);
    
    // Save the averages to processed_data.txt (append to processed_data.txt)
    saveDataToFile(result, "/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/data/processed_data.txt", 0);

    // Clear the raw data file after calculation
    FILE *rawDataFile = fopen("/mnt/c/Users/ideal pc/Desktop/YRO-weather-app/data/raw_data.txt", "w");
    if (rawDataFile) {
        fclose(rawDataFile);  // Empty the raw data file
    } else {
        perror("Failed to clear raw data file");
    }

    // Check for conditions and send alerts
    displayAlert(avgTemp, avgHumidity, avgWindSpeed);
}



// Function to display alerts based on average values and send alerts using system calls
void displayAlert(double avgTemp, double avgHumidity, double avgWindSpeed) {
    // Alert for high temperature
    if (avgTemp > 35.0) {
        printf("Alert: High temperature detected! (%.1f°C)\n", avgTemp);
        system("echo 'High Temperature Alert: Temperature is above 35°C' > /dev/tty");  // Output to terminal
       
    }

    // Alert for high humidity
    if (avgHumidity > 80.0) {
        printf("Alert: High humidity detected! (%.1f%%)\n", avgHumidity);
        system("echo 'High Humidity Alert: Humidity is above 80%' > /dev/tty");  // Output to terminal
  
    }

    // Alert for high wind speed
    if (avgWindSpeed < 15.0) {
        printf("Alert: High wind speed detected! (%.1f m/s)\n", avgWindSpeed);
        system("echo 'High Wind Speed Alert: Wind speed is above 15 m/s' > /dev/tty");  // Output to terminal
       
    }
}


