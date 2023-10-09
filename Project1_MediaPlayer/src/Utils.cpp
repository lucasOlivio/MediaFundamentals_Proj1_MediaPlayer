#include "Utils.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void ErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

namespace myMath
{
    // Wrap number around 1.0 ratio to normalize
    void WrapRatio(float& ratio, const float& currValue, const float& min, const float& max)
    {
        ratio = std::abs(currValue - min) / (max - min);
        return;
    }

    void WrapMinMax(float& num, const float& min, const float& max)
    {
        if (num < min)
        {
            num = min;
            return;
        }
        if (num > max)
        {
            num = max;
            return;
        }
        return;
    }
}

namespace myTime
{
    void FormatTime(unsigned int miliseconds, std::string& formatted)
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

        formatted = formattedTime.str();

        return;
    }
}