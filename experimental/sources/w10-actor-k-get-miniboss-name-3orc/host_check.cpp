// Host contract for getMinibossName__3Orc; not an EE input. Uses the host sprintf.
#include <cstdio>
#include <cstring>
#include "get_miniboss_name.cpp"
int main() {
    static unsigned char o[128];
    int bad = 0;
    const int vals[] = {1, 9, 10, 18, 0, -1, 127, -128};
    for (int v : vals) {
        o[76] = static_cast<unsigned char>(v);
        const char* a = getMinibossName__3Orc(o);
        char want[32]; std::snprintf(want, sizeof want, "MBOrc%d", v);
        if (std::strcmp(a, want) != 0) bad = 1;
        if (getMinibossName__3Orc(o) != a) bad = 1;
    }
    return bad;
}
