#pragma once

#include <iostream>

void ErrorCallback(int error, const char* description);

namespace myMath
{
    // Wrap number around 1.0 ratio to normalize
    void WrapRatio(float& ratio, const float& currValue, const float& min, const float& max);
    // Wrap number between min and max
    void WrapMinMax(float& num, const float& min, const float& max);
}

namespace myTime
{
    // Converts the miliseconds into a human readable string [HH:MM:SS]
    void FormatTime(unsigned int miliseconds, std::string& formatted);
}