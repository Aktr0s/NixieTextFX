#ifndef GET_LOCAL_TIME_HPP
#define GET_LOCAL_TIME_HPP

#include <string>


enum Style {
    CPP_STYLE = true,
    C_STYLE = false
};

/**
 * @enum withSeconds
 * @brief Specifies whether to include seconds in time representation.
 * 
 * This enumeration defines the options for including or excluding seconds.
 */
enum withSeconds {
    WITH_SECONDS = true,    /**< Include seconds in time representation. */
    WITHOUT_SECONDS = false /**< Exclude seconds in time representation. */
};

// Function declaration
std::string getLocalTime(Style style, bool dotBlink, withSeconds withSeconds);

#endif // GET_LOCAL_TIME_HPP
