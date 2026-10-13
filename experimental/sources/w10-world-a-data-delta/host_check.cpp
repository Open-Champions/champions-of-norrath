#include "delta.cpp"
#include <cstring>
int main() {
    if (sizeof(delta) != 4) return 1;
    unsigned w; std::memcpy(&w, &delta, 4); if (w != 0) return 2;
    return 0;
}
