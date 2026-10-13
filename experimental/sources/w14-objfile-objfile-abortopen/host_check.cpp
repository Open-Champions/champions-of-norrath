// Host check only; not a build input.
#include "abortopen.cpp"
#include <cstring>
static void* closed; static int calls; static unsigned char snapshot[640];
static unsigned char* g;
extern "C" int Close__6VIFile(void* f) { closed = f; ++calls; std::memcpy(snapshot, g, sizeof snapshot); return 7; }
int main() {
    alignas(8) unsigned char o[640];
    g = o;
    std::memset(o, 0x5a, sizeof o);
    if (AbortOpen__9VIObjFile(o) != 0 || calls != 1 || closed != o) return 1;
    long long m1 = -1, z = 0;
    for (int i = 0; i < 640; ++i) {
        unsigned char want = 0x5a;
        if (i >= 224 && i < 232) want = reinterpret_cast<unsigned char*>(&m1)[i - 224];
        else if (i >= 216 && i < 224) want = 0;
        else if (i >= 200 && i < 212) want = 0;
        else if (i >= 560 && i < 564) want = 0;
        if (o[i] != want || snapshot[i] != want) return 2;   // everything is stored before Close runs
    }
    (void)z;
    return 0;
}
