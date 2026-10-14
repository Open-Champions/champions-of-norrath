#include "storage.hpp"
int main() {
    if (sizeof(cutInit) != 1) return 1;
    if (cutInit != 0) return 2;
    return 0;
}
