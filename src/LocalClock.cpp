#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>


std::string getLocalTime(Style style, bool dotBlink, withSeconds withSeconds) {
    static std::time_t lastUpdateTime = 0;  // Store the last update time
    static std::string lastTimeStr = "";  // Store the last formatted time string

    // Get the current time
    std::time_t now = std::time(0);
    struct tm* localTime = std::localtime(&now);

    // Check if 1 second has passed since the last update
    if (now != lastUpdateTime) {  // Only proceed if the second has changed
        lastUpdateTime = now;  // Update the last update time

        // Use stringstream to format time
        std::stringstream timeStream;
        if (withSeconds == WITH_SECONDS) {
            timeStream << std::setfill('0')
                       << std::setw(2) << localTime->tm_hour << ":"
                       << std::setw(2) << localTime->tm_min << ":"
                       << std::setw(2) << localTime->tm_sec;
        } else {
            timeStream << std::setfill('0')
                       << std::setw(2) << localTime->tm_hour << ":"
                       << std::setw(2) << localTime->tm_min;
        }

        // Convert the stream to a string
        std::string timeStr = timeStream.str();

        // Handle blinking effect (dotBlink logic)
        if (!dotBlink) {
            if (withSeconds == WITH_SECONDS) {
                timeStr[2] = ' ';  // Replace the first colon
                timeStr[5] = ' ';  // Replace the second colon
            } else {
                timeStr[2] = ' ';  // Replace the colon if seconds are not enabled
            }
        }

        // If the time has changed, update the last time string
        if (timeStr != lastTimeStr) {
            lastTimeStr = timeStr;
            // Return the formatted time string based on the requested style
            if (style == CPP_STYLE) {
                return timeStr;  // Return as C++ string
            } else {
                return std::string(timeStr.c_str());  // Return as C-style string
            }
        }
    }

    // If the time hasn't changed, return the last stored time
    return lastTimeStr;
}
