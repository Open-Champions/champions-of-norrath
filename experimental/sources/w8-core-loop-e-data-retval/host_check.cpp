#include "retval.cpp"
int main() {
    static_assert(sizeof(retval) == 64, "64 bytes");
    if (reinterpret_cast<unsigned long>(retval) % 64 != 0) return 1;
    for (unsigned char b : retval) if (b != 0) return 2;
    return 0;
}
