#include "draw.cpp"

#include <cstdio>

int main() {
    for (unsigned word : draw) {
        if (word != 0u) return 1;
    }
    draw[15] = 1u;
    if (draw[15] == 0u || draw[0] != 0u) return 2;
    draw[15] = 0u;
    std::puts("draw contract passed");
    return 0;
}
