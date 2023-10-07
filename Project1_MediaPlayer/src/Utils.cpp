#include "Utils.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void ErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

namespace myTime
{
    std::string FormatTime(unsigned int miliseconds)
    {
        // Convert time to each needed unit
        unsigned int seconds = miliseconds / 1000;
        unsigned int hours = seconds / 3600;
        seconds %= 3600;
        unsigned int minutes = seconds / 60;
        seconds %= 60;

        // Sets time to human readable format (HH:MM:SS)
        std::stringstream formattedTime;
        formattedTime << std::setfill('0') << std::setw(2) << hours << ":"
                        << std::setfill('0') << std::setw(2) << minutes << ":"
                        << std::setfill('0') << std::setw(2) << seconds;

        return formattedTime.str();
    }
}