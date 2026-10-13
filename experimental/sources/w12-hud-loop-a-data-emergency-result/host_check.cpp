// Host check for the emergencyResult object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "data_emergency_result.cpp"

#include <cstddef>

int main() {
    if (sizeof(emergencyResult) != 1568) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&emergencyResult);
    for (std::size_t i = 0; i < sizeof(emergencyResult); ++i)
        if (bytes[i] != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&emergencyResult) % alignof(int) != 0) return 3;
    return 0;
}
