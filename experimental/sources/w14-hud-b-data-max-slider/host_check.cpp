// Host check for the maxSlider object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_max_slider.cpp"

int main() {
    if (sizeof(maxSlider) != 1) return 1;
    if (maxSlider != 0) return 2;
    return 0;
}
