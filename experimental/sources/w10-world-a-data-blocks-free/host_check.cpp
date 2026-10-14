#include "blocks_free.cpp"
#include <cstring>
int main() {
    if (sizeof(blocksFree) != 4) return 1;
    if (blocksFree != 0) return 2;
    return 0;
}
