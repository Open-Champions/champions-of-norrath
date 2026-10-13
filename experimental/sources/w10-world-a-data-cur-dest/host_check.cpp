#include "cur_dest.cpp"
#include <cstring>
int main() {
    if (sizeof(curDest) != 1) return 1;
    if (curDest != 0) return 2;
    return 0;
}
