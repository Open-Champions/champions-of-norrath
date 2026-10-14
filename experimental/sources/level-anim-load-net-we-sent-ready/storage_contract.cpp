#include "storage.hpp"
int main() {
    if (sizeof(netWeSentReady) != 4) return 1;
    if (netWeSentReady != 0) return 2;
    return 0;
}
