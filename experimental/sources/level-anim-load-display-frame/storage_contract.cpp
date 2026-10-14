#include "storage.hpp"
int main() {
    if (sizeof(displayFrame) != 4) return 1;
    if (displayFrame != 0) return 2;
    return 0;
}
