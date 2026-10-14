// Host check only; not a build input.
#include <cstring>
static int reads, lastCount;
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) { ++reads; lastCount = count; if (count > 0) std::memset(buffer, 0x5a, count); return count; }
#include "read_bytes.cpp"
int main() {
    alignas(8) unsigned char f[600] = {};
    unsigned char buf[16] = {};
    *reinterpret_cast<int*>(f + 232) = 1;
    int* remaining = reinterpret_cast<int*>(f + 1 * 32 + 264);
    *remaining = 10;
    if (Read__9VIObjFilePUci(f, buf, 4) != -1 || reads != 0) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    if (Read__9VIObjFilePUci(f, buf, 11) != -1 || reads != 0 || *remaining != 10) return 2;
    if (Read__9VIObjFilePUci(f, buf, 10) != 0 || reads != 1 || lastCount != 10 || *remaining != 0 || buf[9] != 0x5a) return 3;
    *remaining = 3;
    if (Read__9VIObjFilePUci(f, buf, 0) != 0 || *remaining != 3 || lastCount != 0) return 4;
    if (Read__9VIObjFilePUci(f, buf, -2) != 0 || *remaining != 5) return 5;
    return 0;
}
