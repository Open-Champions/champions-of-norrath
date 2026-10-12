#include "storage.hpp"
int main() {
    if (sizeof(cutSceneName) != 80) return 1;
    for (unsigned i = 0; i != 80; ++i) if (cutSceneName[i] != 0) return 2;
    return 0;
}
