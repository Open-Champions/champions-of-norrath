// Host check for the bodyAlpha object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_body_alpha.cpp"

int main() {
    if (sizeof(bodyAlpha) != 4) return 1;
    if (bodyAlpha != 0) return 2;
    return 0;
}
