#include "clearing.cpp"
#include <cstring>
int main() {
    if (sizeof(clearing) != 1) return 1;
    if (clearing != 0) return 2;
    return 0;
}
