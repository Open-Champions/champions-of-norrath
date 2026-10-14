#include "storage.hpp"
int main() {
    if (sizeof(displayActionPos) != 4) return 1;
    if (displayActionPos != 0) return 2;
    return 0;
}
