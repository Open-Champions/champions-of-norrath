#include "storage.hpp"
int main() {
    if (sizeof(displayCutPos) != 4) return 1;
    if (displayCutPos != 0) return 2;
    return 0;
}
