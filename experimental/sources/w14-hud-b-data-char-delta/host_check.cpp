// Host check for the charDelta object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_char_delta.cpp"

int main() {
    if (sizeof(charDelta) != 4) return 1;
    if (charDelta != 0) return 2;
    return 0;
}
