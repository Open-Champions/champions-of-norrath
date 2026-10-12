// Host check for temp_buffer.cpp; not an EE plan input.
#include "temp_buffer.cpp"

int main() {
    if (tempBuffer[0] != 0 || tempBuffer[95935] != 0) return 1;
    tempBuffer[95935] = 3;
    if (tempBuffer[95935] != 3) return 2;
    return 0;
}
