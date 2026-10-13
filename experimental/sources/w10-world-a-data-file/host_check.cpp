#include "file.cpp"
#include <cstring>
int main() {
    if (sizeof(file) != 4) return 1;
    if (file != 0) return 2;
    return 0;
}
