#include "cache_load_delay_time.cpp"
#include <cstring>
int main() {
    if (sizeof(cacheLoadDelayTime) != 4) return 1;
    if (cacheLoadDelayTime != 0) return 2;
    return 0;
}
