#pragma once

#include <iostream>

void ErrorCallback(int error, const char* description);

namespace myTime
{
    // Converts the miliseconds into a human readable string [HH:MM:SS]
    std::string FormatTime(unsigned int miliseconds);
}