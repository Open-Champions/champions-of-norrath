// Host check for the lastSlider object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_last_slider.cpp"

int main() {
    if (sizeof(lastSlider) != 1) return 1;
    if (lastSlider != 0) return 2;
    return 0;
}
