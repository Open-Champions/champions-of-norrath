#include "g_lib_data.cpp"
int main() {
    if (sizeof(GLibData) != 1640) return 1;
    for (unsigned char b : GLibData) if (b != 0) return 2;
    return 0;
}
