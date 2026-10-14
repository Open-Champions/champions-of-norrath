#include "storage.hpp"
int main() {
    if (sizeof(cutSkipMode) != 1) return 1;
    if (cutSkipMode != 0) return 2;
    return 0;
}
