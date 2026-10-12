#include "storage.hpp"
int main() {
    if (sizeof(netNmReady) != 4) return 1;
    if (netNmReady != 0) return 2;
    return 0;
}
