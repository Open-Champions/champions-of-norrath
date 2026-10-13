// Host check for the curSlider object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_cur_slider.cpp"

int main() {
    if (sizeof(curSlider) != 1) return 1;
    if (curSlider != 0) return 2;
    return 0;
}
