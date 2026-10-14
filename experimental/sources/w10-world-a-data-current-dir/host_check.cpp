#include "current_dir.cpp"
#include <cstring>
int main() {
    if (sizeof(currentDir) != 4096) return 1;
    for (unsigned i = 0; i < sizeof(currentDir); i++) if (((const unsigned char*)&currentDir[0])[i] != 0) return 2;
    return 0;
}
