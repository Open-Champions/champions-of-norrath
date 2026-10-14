// Host check for the drewHelpfulMessageHelpBar object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_drew_helpful_message_help_bar.cpp"

#include <cstddef>

int main() {
    if (sizeof(drewHelpfulMessageHelpBar) != 4) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&drewHelpfulMessageHelpBar);
    for (std::size_t i = 0; i < sizeof(drewHelpfulMessageHelpBar); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&drewHelpfulMessageHelpBar) % alignof(int) != 0) return 3;
    return 0;
}
