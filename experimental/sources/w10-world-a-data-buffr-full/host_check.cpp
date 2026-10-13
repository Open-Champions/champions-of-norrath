#include "buffr_full.cpp"
#include <cstring>
int main() {
    if (sizeof(buffrfull) != 4) return 1;
    if (buffrfull != 0) return 2;
    return 0;
}
