#include "storage.hpp"
int main() {
    if (sizeof(cutPlayMode) != 1) return 1;
    if (cutPlayMode != 0) return 2;
    return 0;
}
