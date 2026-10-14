#include "cache_load_valid.cpp"
#include <cstring>
int main() {
    if (sizeof(cacheLoadValid) != 4) return 1;
    if (cacheLoadValid != 0) return 2;
    return 0;
}
