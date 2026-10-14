#include "getmode.cpp"
#include <initializer_list>
int main() {
    alignas(4) unsigned char cam[96];
    for (int i = 0; i < 96; ++i) cam[i] = static_cast<unsigned char>(i + 1);
    for (int v : {0, 1, -1, 7, 0x12345678}) {
        *reinterpret_cast<int*>(cam + 52) = v;
        if (GetMode__6Camera(cam) != v) return 1;
    }
    // neighbouring words must not be what is returned
    *reinterpret_cast<int*>(cam + 48) = 11; *reinterpret_cast<int*>(cam + 56) = 22; *reinterpret_cast<int*>(cam + 52) = 33;
    return GetMode__6Camera(cam) == 33 ? 0 : 2;
}
