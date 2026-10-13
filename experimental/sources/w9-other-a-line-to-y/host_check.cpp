#include "line_to_y.cpp"
int main() {
    if (lineToY__Fi(0) != 295) return 1;
    if (lineToY__Fi(1) != 313) return 2;
    if (lineToY__Fi(-1) != 277) return 3;
    if (lineToY__Fi(10) != 475) return 4;
    return 0;
}
