#ifndef GET_LOCAL_TIME_HPP
#define GET_LOCAL_TIME_HPP

#include <string>

// Enumerations for style and seconds options
enum Style {
    CPP_STYLE = true,
    C_STYLE = false
};

enum withSeconds {
    WITH_SECONDS = true,
    WITHOUT_SECONDS = false
};

// Function declaration
std::string getLocalTime(Style style, bool dotBlink, withSeconds withSeconds);

#endif // GET_LOCAL_TIME_HPP
