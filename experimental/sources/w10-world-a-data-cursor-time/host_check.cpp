#include "cursor_time.cpp"
#include <cstring>
int main() {
    if (sizeof(cursorTime) != 4) return 1;
    unsigned w; std::memcpy(&w, &cursorTime, 4); if (w != 0) return 2;
    return 0;
}
