// Host check for the msgAlpha object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_msg_alpha.cpp"

int main() {
    if (sizeof(msgAlpha) != 4) return 1;
    if (msgAlpha != 0) return 2;
    return 0;
}
