#include "status_ptr.cpp"
int main() {
    static_assert(sizeof(statusPtr) == 4, "one word");
    if (statusPtr != 0) return 1;
    statusPtr = 0x12345678u;
    return statusPtr == 0x12345678u ? 0 : 2;
}
