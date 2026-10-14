#include "buffr_mode.cpp"
#include <cstring>
int main() {
    if (sizeof(buffrmode) != 4) return 1;
    if (buffrmode != 0) return 2;
    return 0;
}
