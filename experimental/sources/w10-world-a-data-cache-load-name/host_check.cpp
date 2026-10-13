#include "cache_load_name.cpp"
#include <cstring>
int main() {
    if (sizeof(cacheLoadName) != 64) return 1;
    for (unsigned i = 0; i < sizeof(cacheLoadName); i++) if (((const unsigned char*)&cacheLoadName[0])[i] != 0) return 2;
    return 0;
}
