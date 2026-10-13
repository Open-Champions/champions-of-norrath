#include "load_buffer.cpp"
int main() {
    static_assert(sizeof(loadBuffer) == 60032, "gap to tempBuffer");
    for (unsigned char byte : loadBuffer) if (byte != 0) return 1;
    return 0;
}
