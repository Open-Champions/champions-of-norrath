// Host check only; not a build input.
#include <string>
#include <vector>
#include "buffered_read.cpp"
static std::string tr; static std::vector<int> copyRet; static size_t copyCalls; static int syncRet, cdReadRet;
template <class T> static T& at(unsigned char* f, int off) { return *reinterpret_cast<T*>(f + off); }
static unsigned char* gFile;
extern "C" int sceLseek(int, int, int) { tr += "seek;"; return 0; }
extern "C" int sceWrite(int, unsigned, int) { tr += "write;"; return 0; }
extern "C" int BufferClear__6VIFile(unsigned char* f) { tr += "C;"; for (int o : {136, 176, 144, 152, 160}) at<long long>(f, o) = 0; at<int>(f, 168) = 0; return 0; }
extern "C" int BufferedReadCopy__6VIFilePvi(unsigned char* f, unsigned char*, int n) {
    int v = copyCalls < copyRet.size() ? copyRet[copyCalls] : n; ++copyCalls;
    if (v > n) v = n;
    tr += "copy" + std::to_string(n) + ">" + std::to_string(v) + ";"; at<long long>(f, 16) += v; return v;
}
extern "C" int CDSync__6VIFilei(unsigned char*, int mode) { tr += "sync" + std::to_string(mode) + ";"; return syncRet; }
extern "C" int CDRead__6VIFileUiUiPvP10sceCdRMode(unsigned char* f, unsigned sector, unsigned sectors, unsigned, unsigned char* mode) {
    tr += "read" + std::to_string(sector) + "," + std::to_string(sectors) + (mode == f + 108 ? "" : "?") + ";"; return cdReadRet;
}
int main() {
    alignas(8) unsigned char f[200] = {};
    unsigned char out[70000];
    auto run = [&](int n) { tr.clear(); copyCalls = 0; return BufferedRead__6VIFilePvi(f, out, n); };
    at<int>(f, 72) = 1000; at<unsigned>(f, 76) = 100000; syncRet = 0; cdReadRet = 1;
    // everything already cached
    copyRet = {50};
    if (run(50) != 50 || tr != "copy50>50;" || at<int>(f, 8) != 0) return 1;
    // end-of-file flag with bytes still wanted
    copyRet = {10}; at<int>(f, 168) = 1;
    if (run(50) != 10 || at<int>(f, 8) != 1) return 2;
    at<int>(f, 168) = 0; at<int>(f, 8) = 0;
    // aligned read of 5000 bytes at position 4096 with an empty cache: two whole sectors then a tail sector
    at<long long>(f, 16) = 4096; copyRet = {0, 4096, 904};
    int r = run(5000);
    // the body reads two sectors and copies them out; the 904-byte remainder reads a sector run of up to 16 starting at sector 4
    if (r != 5000 || tr != "copy5000>0;C;sync0;read1002,2;sync0;copy5000>4096;C;sync0;read1004,16;sync0;copy904>904;") return 3;
    // unaligned head: position 100 reads one sector first
    at<long long>(f, 16) = 100; copyRet = {0, 1948}; tr.clear();
    r = run(1948);
    if (r != 1948 || tr.compare(0, 24, "copy1948>0;C;sync0;read1") != 0) return 4;
    if (at<long long>(f, 136) != 0 || at<long long>(f, 176) != 2048) return 5;
    // failures
    at<long long>(f, 16) = 0; copyRet = {0}; syncRet = 1;
    if (run(100) != -1) return 6;
    syncRet = 0; cdReadRet = 0;
    if (run(100) != -1) return 7;
    // position at the end of the file
    cdReadRet = 1; at<unsigned>(f, 76) = 2048; at<long long>(f, 16) = 2048; copyRet = {0}; at<int>(f, 8) = 0;
    if (run(100) != 0 || at<int>(f, 8) != 1) return 8;
    return 0;
}
