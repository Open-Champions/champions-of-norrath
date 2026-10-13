#include "buffr_out.cpp"
#include <cstring>
int main() {
    if (sizeof(buffrout) != 4) return 1;
    if (buffrout != 0) return 2;
    return 0;
}
