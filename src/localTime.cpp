#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

enum Style {
    CPP_STYLE = true,
    C_STYLE = false
};

std::string getLocalTime(Style style, bool dotBlink) {
    // Get current time
    std::time_t now = std::time(0);
    struct tm* localTime = std::localtime(&now);

    // Use stringstream to format time
    std::stringstream timeStream;
    timeStream << std::setfill('0') 
               << std::setw(2) << localTime->tm_hour << ":"
               << std::setw(2) << localTime->tm_min << ":"
               << std::setw(2) << localTime->tm_sec;

    // Convert the stream to a string
    std::string timeStr = timeStream.str();

    // Handle blinking effect
    if (!dotBlink) {
        timeStr[2] = ' '; // Replace first colon
        timeStr[5] = ' '; // Replace second colon
    }

    if (style == CPP_STYLE) {
        return timeStr; // Return as C++ string
    } else {
        return std::string(timeStr.c_str()); // Ensure null-terminated C-style compatibility
    }
}
