// Host check for the msgAlphaDelta object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_msg_alpha_delta.cpp"

int main() {
    if (sizeof(msgAlphaDelta) != 4) return 1;
    if (msgAlphaDelta != 0) return 2;
    return 0;
}
