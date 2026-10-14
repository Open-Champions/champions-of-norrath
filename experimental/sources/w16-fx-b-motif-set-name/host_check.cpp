// Host check only; not a build input.
#include "set_name.cpp"
#include <cstring>
static int order[4], n;
extern "C" char* strcpy(char* d, const char* s) { order[n++] = 1; std::memcpy(d, s, std::strlen(s) + 1); return d; }
extern "C" unsigned VIHashResourceID(const char* s) { order[n++] = 2; unsigned h = 0; while (*s) h = h * 31 + (unsigned char)*s++; return h; }
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0, sizeof obj);
    SetName__15VIParticleMotifPCc(obj, "ember");
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (std::strcmp(obj + 724, "ember") != 0) return 2;
    unsigned h = 0; for (const char* p = "ember"; *p; p++) h = h * 31 + (unsigned char)*p;
    if (*(unsigned*)(obj + 756) != h) return 3;
    return 0;
}
