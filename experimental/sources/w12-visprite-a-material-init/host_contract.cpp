#include "init.cpp"
#include <cstring>
#include <vector>

static std::vector<const void*> idents;
extern "C" void Identity__10VIMatrix33(void* m) { idents.push_back(m); }

static void w32(unsigned char* b, int off, unsigned v) { std::memcpy(b + off, &v, 4); }

int main() {
    for (int layers : {-1, 0, 1, 3}) {
        alignas(8) unsigned char object[48 + 144 * 4 + 16];
        std::memset(object, 0x5a, sizeof object);
        alignas(8) unsigned char want[sizeof object];
        std::memcpy(want, object, sizeof object);
        idents.clear();
        w32(want, 12, 19); w32(want, 8, layers); w32(want, 16, 0); w32(want, 32, 1);
        for (int i = 0; i < layers; ++i) {
            unsigned char* r = want + 48 + 144 * i;
            w32(r, 0, 0xffffffffu); w32(r, 4, 1); w32(r, 8, 0xffffffffu); w32(r, 12, 0);
            w32(r, 52, 0x3f4cccccu); w32(r, 56, 0); w32(r, 60, 0); w32(r, 64, 0); w32(r, 68, 0); w32(r, 72, 0xffffffffu);
        }
        if (Init__10VIMateriali(object, layers) != 0) return 1;
        if (std::memcmp(want, object, sizeof object) != 0) return 2;
        if (static_cast<int>(idents.size()) != (layers > 0 ? layers : 0)) return 3;
        for (int i = 0; i < layers; ++i)
            if (idents[i] != object + 48 + 144 * i + 16) return 4;
    }
    return 0;
}
