#include "storage.hpp"
int main() {
    if (sizeof(cutTime) != 4) return 1;
    if (cutTime != 0) return 2;
    return 0;
}
