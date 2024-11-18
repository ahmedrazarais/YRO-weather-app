#ifndef WEATHER_H
#define WEATHER_H

#include <curl/curl.h>
#include <cjson/cJSON.h>

#define MAX_API_CALLS 5

// Structure to store weather data
struct WeatherData {
    double temp;
    double feels_like;
    int humidity;
    double wind_speed;
};

// Structure for CURL response
struct string {
    char *ptr;
    size_t len;
};

// Function declarations
void fetchWeatherData(const char *city);
void displayAlert(double avgTemp, double avgHumidity, double avgWindSpeed);
void calculateAndSaveAverages(void);
void saveDataToFile(const char *data, const char *filename, int isRawData);
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

#endif // WEATHER_H
