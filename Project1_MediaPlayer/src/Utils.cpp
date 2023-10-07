#include "Utils.h"

void ErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}