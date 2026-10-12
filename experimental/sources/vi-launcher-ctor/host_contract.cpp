#include "launcher_ctor.cpp"

#include <cstring>

int main() {
    unsigned char object[32];
    std::memset(object, 0xa5, sizeof object);
    __10VILauncher(reinterpret_cast<VILauncher*>(object));
    for (unsigned char byte : object) {
        if (byte != 0xa5) return 1;
    }
    return 0;
}
