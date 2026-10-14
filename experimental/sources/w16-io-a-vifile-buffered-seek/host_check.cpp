// Host check only; not a build input.
#include <string>
#include "buffered_seek.cpp"
static std::string tr; static long long lseekRet; static int writeRet;
extern "C" int sceLseek(int fd, int off, int whence) { tr += "L" + std::to_string(fd) + "," + std::to_string(off) + "," + std::to_string(whence) + ";"; return static_cast<int>(lseekRet); }
extern "C" int sceWrite(int fd, unsigned address, int size) { tr += "W" + std::to_string(fd) + "," + std::to_string(address) + "," + std::to_string(size) + ";"; return writeRet; }
extern "C" int BufferClear__6VIFile(unsigned char*) { tr += "C;"; return 0; }
template <class T> static T& at(unsigned char* f, int off) { return *reinterpret_cast<T*>(f + off); }
int main() {
    alignas(8) unsigned char f[200] = {};
    auto seek = [&](long off, int origin) { tr.clear(); return BufferedSeek__6VIFilelQ26VIFile10SeekOrigin(f, off, origin); };
    at<int>(f, 0) = 7; at<unsigned>(f, 76) = 4000; at<unsigned>(f, 188) = 0x5000;
    at<long long>(f, 136) = 1000; at<long long>(f, 176) = 64;
    at<int>(f, 8) = 9;
    // inside the buffer
    if (seek(1010, 1) != 0 || at<long long>(f, 16) != 1010 || at<long long>(f, 160) != 10 || at<int>(f, 8) != 0 || !tr.empty()) return 1;
    if (seek(5, 0) != 0 || at<long long>(f, 16) != 1015 || at<long long>(f, 160) != 15) return 2;
    if (seek(-4000, 2) != 0 && at<long long>(f, 16) != 0) return 3;
    // seek relative to the size, landing outside the buffer with no dirty range
    at<long long>(f, 144) = at<long long>(f, 152) = 0;
    if (seek(-100, 2) != 0 || at<long long>(f, 16) != 3900 || tr != "C;") return 4;
    // dirty range written back: target 1000 + 8 = 1008, current descriptor position differs
    at<long long>(f, 144) = 8; at<long long>(f, 152) = 24; at<long long>(f, 24) = 0;
    lseekRet = 1008; writeRet = 16;
    if (seek(3000, 1) != 0 || tr != "L7,1008,0;W7,20488,16;C;") return 5;
    if (at<long long>(f, 24) != 1024 || at<long long>(f, 144) != 0 || at<long long>(f, 152) != 0) return 6;
    // descriptor already at the target: no lseek; a short write keeps the dirty range
    at<long long>(f, 144) = 8; at<long long>(f, 152) = 24; at<long long>(f, 24) = 1008; writeRet = 10;
    if (seek(3000, 1) != 0 || tr != "W7,20488,16;C;" || at<long long>(f, 24) != 1018 || at<long long>(f, 152) != 24) return 7;
    // lseek lands elsewhere: no write
    at<long long>(f, 24) = 0; lseekRet = 5;
    if (seek(3000, 1) != 0 || tr != "L7,1008,0;C;" || at<long long>(f, 24) != 5) return 8;
    // empty buffer and bad origin
    at<long long>(f, 176) = 0; at<int>(f, 8) = 3;
    if (seek(10, 1) != 0 || at<int>(f, 8) != 0 || !tr.empty()) return 9;
    at<long long>(f, 16) = 77;
    if (seek(10, 5) != -1 || at<long long>(f, 16) != 77) return 10;
    return 0;
}
