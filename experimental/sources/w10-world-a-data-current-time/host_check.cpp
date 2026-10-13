#include "current_time.cpp"
#include <cstring>
int main() {
    if (sizeof(currentTime) != 4) return 1;
    unsigned w; std::memcpy(&w, &currentTime, 4); if (w != 0) return 2;
    return 0;
}
