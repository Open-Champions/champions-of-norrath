#include "last_y_anchor.cpp"
int main() {
    if (sizeof(lastYAnchor) != 4) return 1;
    if (lastYAnchor != 0) return 2;
    lastYAnchor = 1;
    if (lastYAnchor != 1) return 3;
    return 0;
}
