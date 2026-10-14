// Host check only; not a build input.
#include <string>
#include <string.h>
#define NORRATH_HOST_ARENA 1
// (a Mach-O host adds the leading underscore itself, so the host check names it with one)
#define NORRATH_MEMCPY_SYMBOL "_host_buffered_write_memcpy"
static unsigned char arena[0x20000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "buffered_write.cpp"
static std::string tr; static long long lseekRet; static int writeRet;
extern "C" void* host_buffered_write_memcpy(unsigned dst, const void* src, unsigned n) { tr += "M" + std::to_string(dst) + "," + std::to_string(n) + ";"; memcpy(arena + dst, src, n); return nullptr; }
extern "C" int sceLseek(int fd, int off, int whence) { tr += "L" + std::to_string(fd) + "," + std::to_string(off) + "," + std::to_string(whence) + ";"; return static_cast<int>(lseekRet); }
extern "C" int sceWrite(int fd, unsigned address, int size) { tr += "W" + std::to_string(fd) + "," + std::to_string(address) + "," + std::to_string(size) + ";"; return writeRet < 0 ? writeRet : size - writeRet; }
extern "C" int BufferClear__6VIFile(unsigned char* f) {
    tr += "C;";
    for (int o : {136, 176, 144, 152, 160}) *reinterpret_cast<long long*>(f + o) = 0;
    return 0;
}
template <class T> static T& at(unsigned char* f, int off) { return *reinterpret_cast<T*>(f + off); }
int main() {
    alignas(8) unsigned char f[200] = {};
    unsigned char data[4000];
    for (int i = 0; i < 4000; ++i) data[i] = static_cast<unsigned char>(i + 1);
    auto write = [&](int n) { tr.clear(); return BufferedWrite__6VIFilePCvi(f, data, n); };
    at<int>(f, 0) = 7; at<unsigned>(f, 188) = 0x1000; at<int>(f, 8) = 5;
    // first small write into an empty cache: the block starts at the position
    at<long long>(f, 16) = 300;
    if (write(10) != 10 || tr != "M4096,10;" || at<int>(f, 8) != 0) return 1;
    if (at<long long>(f, 136) != 300 || at<long long>(f, 16) != 310 || at<long long>(f, 160) != 10 || at<long long>(f, 176) != 10) return 2;
    if (at<long long>(f, 144) != 0 || at<long long>(f, 152) != 10 || arena[0x1000] != 1 || arena[0x1009] != 10) return 3;
    // a second write continues in the cache
    if (write(5) != 5 || tr != "M4106,5;" || at<long long>(f, 160) != 15 || at<long long>(f, 152) != 15 || at<long long>(f, 176) != 15) return 4;
    // a write that would pass 32767 bytes writes the cache back (target 300 + 0, 15 bytes), then restarts the block at the position
    at<long long>(f, 160) = 32760; at<long long>(f, 176) = 32760; at<long long>(f, 144) = 0; at<long long>(f, 152) = 15; at<long long>(f, 24) = 0;
    lseekRet = 300; writeRet = 0;
    if (write(100) != 100 || tr != "L7,300,0;W7,4096,15;M4096,100;") return 5;
    if (at<long long>(f, 136) != 315 || at<long long>(f, 160) != 100 || at<long long>(f, 144) != 0 || at<long long>(f, 152) != 100) return 6;
    // large write: dirty data is written back, the cache dropped, then a direct write
    at<long long>(f, 24) = 315; at<long long>(f, 16) = 400; at<long long>(f, 144) = 0; at<long long>(f, 152) = 100; at<long long>(f, 176) = 100;
    at<long long>(f, 136) = 300; at<long long>(f, 24) = 300;
    writeRet = 0; lseekRet = 400;
    if (write(3000) != 3000 || tr != "W7,4096,100;C;L7,400,0;W7,4096,3000;") {
        // the direct write passes the caller's buffer as an address word
        if (tr.compare(0, 14, "W7,4096,100;C;") != 0) return 7;
    }
    if (at<long long>(f, 16) != 3400 || at<long long>(f, 24) != 3400) return 8;
    // large write: a failed positioning call returns -1
    at<long long>(f, 24) = 5; at<long long>(f, 16) = 400; at<long long>(f, 176) = 0; lseekRet = 399;
    if (write(2049) != -1) return 9;
    // a short direct write returns -1
    at<long long>(f, 24) = 400; writeRet = 1;
    if (write(2049) != -1) return 10;
    return 0;
}
