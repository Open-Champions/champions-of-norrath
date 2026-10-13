// Host contract for getModel__3NPC; not an EE input. Returns non-zero when wrong.
#include "get_model.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char object[2048];
    int bad = 0;
    for (unsigned long long v : {0ull, 0x1234ull, 0xdeadbeefull}) {
        void* p = reinterpret_cast<void*>(static_cast<unsigned long>(v));
        std::memset(object, 0x5a, sizeof object);
        std::memcpy(object + 1056, &p, sizeof p);
        if (getModel__3NPC(object) != p) bad = 1;
    }
    return bad;
}
