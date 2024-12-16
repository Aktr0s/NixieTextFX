#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

enum Style {
    CPP_STYLE = true,
    C_STYLE = false
};

enum withSeconds {
    WITH_SECONDS = true,
    WITHOUT_SECONDS = false
};

std::string getLocalTime(Style style, bool dotBlink, withSeconds withSeconds) {
    // Get current time
    std::time_t now = std::time(0);
    struct tm* localTime = std::localtime(&now);

    // Use stringstream to format time
    std::stringstream timeStream;
    if (withSeconds) {
        timeStream << std::setfill('0') 
                   << std::setw(2) << localTime->tm_hour << ":"
                   << std::setw(2) << localTime->tm_min << ":"
                   << std::setw(2) << localTime->tm_sec;
    } else if (!withSeconds) {
        timeStream << std::setfill('0')
                   << std::setw(2) << localTime->tm_hour << ":"
                   << std::setw(2) << localTime->tm_min;
    }
    
    // Convert the stream to a string
    std::string timeStr = timeStream.str();

    // Handle blinking effect
    if (!dotBlink) {
        if (withSeconds){
            timeStr[2] = ' '; // Replace first colon
            timeStr[5] = ' '; // Replace second colon
        } else if (!withSeconds){
            timeStr[2] = ' ';
        }
    }   

    if (style == CPP_STYLE) {
        return timeStr; // Return as C++ string
    } else {
        return std::string(timeStr.c_str()); // Ensure null-terminated C-style compatibility
    }
}
