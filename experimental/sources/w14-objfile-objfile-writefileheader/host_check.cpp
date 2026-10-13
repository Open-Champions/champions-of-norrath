// Host check only; not a build input.
#include "writefileheader.cpp"
#include <cstdio>
#include <cstring>
#include <string>
extern "C" int VIEndian = 1;
static std::string trace; static void* obj; static long long tell_value;
extern "C" long long Tell__6VIFile(void* f) { trace += f == obj ? "tell;" : "tell?;"; return tell_value; }
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void* f, long long off, int origin) { trace += "seek" + std::to_string(off) + "," + std::to_string(origin) + (f == obj ? ";" : "?;"); return 0; }
extern "C" int Write__6VIFilePCvi(void* f, const void* buf, int n) { trace += (f == obj && buf == static_cast<unsigned char*>(obj) + 200) ? "write" + std::to_string(n) + ";" : "write?;"; return n; }
extern "C" void VIBufferSwap__FiPUc(int n, unsigned char* p) { trace += "swap" + std::to_string(n) + "@" + std::to_string(p - static_cast<unsigned char*>(obj)) + ";"; }
int main() {
    bool ok = true;
    alignas(8) unsigned char o[640];
    obj = o;
    const int endians[] = {1, 0, 2};
    const long long positions[] = {0LL, 96LL};
    for (int endian : endians)
        for (long long pos : positions)
            for (int restore = 0; restore < 2; ++restore) {
                VIEndian = endian; tell_value = pos; trace.clear();
                std::memset(o, 0, sizeof o);
                int r = WriteFileHeader__9VIObjFilei(o, restore);
                std::string swaps = "swap4@200;swap4@204;swap4@208;swap8@216;swap8@224;";
                std::string want = "tell;";
                if (pos) want += "seek0,1;";
                if (endian != 1) want += swaps;
                want += "write32;";
                if (endian != 1) want += swaps;
                if (restore) want += "seek" + std::to_string(pos) + ",1;";
                if (r != 0 || trace != want) { std::puts(trace.c_str()); ok = false; }
            }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
