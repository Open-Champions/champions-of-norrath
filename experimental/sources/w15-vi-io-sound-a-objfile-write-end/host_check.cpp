// Host check only; not a build input.
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> log_;
static long long tellResult;
extern "C" long long Tell__6VIFile(unsigned char*) { return tellResult; }
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char*, long long offset, int origin) { log_.push_back("seek" + std::to_string(offset) + "," + std::to_string(origin)); return 0; }
extern "C" int Write__6VIFilePCvi(unsigned char*, const void* buffer, int count) {
    unsigned w[2];
    std::memcpy(w, buffer, 8);
    log_.push_back("write" + std::to_string(count) + ":" + std::to_string(w[0]) + "," + std::to_string(w[1]));
    return count;
}
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    log_.push_back("swap" + std::to_string(size));
    for (int i = 0; i < size / 2; ++i) { unsigned char t = data[i]; data[i] = data[size - 1 - i]; data[size - 1 - i] = t; }
}
extern "C" { int VIEndian; }
#include "write_end.cpp"
int main() {
    alignas(8) unsigned char f[700] = {};
    unsigned size = 0;
    VIEndian = 1;
    tellResult = 500;
    *reinterpret_cast<int*>(f + 232) = 1;
    if (WriteEnd__9VIObjFileRUi(f, &size) != -1 || !log_.empty()) return 1;
    *reinterpret_cast<int*>(f + 572) = 1;
    *reinterpret_cast<int*>(f + 204) = 7;
    // entry 1 (child) of a parent entry 0
    unsigned char* parent = f + 240;
    unsigned char* child = f + 240 + 32;
    *reinterpret_cast<unsigned*>(parent + 4) = 100;
    *reinterpret_cast<unsigned*>(parent + 8) = 2;
    *reinterpret_cast<long long*>(child + 16) = 300;
    *reinterpret_cast<unsigned*>(child + 4) = 40;
    *reinterpret_cast<unsigned*>(child + 8) = 3;
    *reinterpret_cast<int*>(f + 580) = 9;
    if (WriteEnd__9VIObjFileRUi(f, &size) != 0) return 2;
    if (size != 52 || *reinterpret_cast<int*>(f + 232) != 0 || *reinterpret_cast<int*>(f + 572) != 1 || *reinterpret_cast<int*>(f + 204) != 7) return 3;
    if (*reinterpret_cast<unsigned*>(parent + 4) != 100 + 40 + 12 || *reinterpret_cast<unsigned*>(parent + 8) != 3) return 4;
    if (*reinterpret_cast<int*>(f + 580) != 0) return 5;
    if (log_.size() != 3 || log_[0] != "seek304,1" || log_[1] != "write8:40,3" || log_[2] != "seek500,1") return 6;
    // outermost: completes the top-level object and clears +572
    log_.clear();
    *reinterpret_cast<long long*>(parent + 16) = 64;
    *reinterpret_cast<unsigned*>(parent + 4) = 152;
    *reinterpret_cast<unsigned*>(parent + 8) = 3;
    if (WriteEnd__9VIObjFileRUi(f, &size) != 0 || size != 164 || *reinterpret_cast<int*>(f + 232) != -1) return 7;
    if (*reinterpret_cast<int*>(f + 572) != 0 || *reinterpret_cast<int*>(f + 204) != 8) return 8;
    if (log_.size() != 3 || log_[0] != "seek68,1" || log_[1] != "write8:152,3") return 9;
    // other byte order: swapped before and after the write, the file receives the swapped words, memory ends unswapped
    log_.clear();
    VIEndian = 0;
    *reinterpret_cast<int*>(f + 572) = 1;
    *reinterpret_cast<int*>(f + 232) = 0;
    *reinterpret_cast<unsigned*>(parent + 4) = 0x01020304u;
    *reinterpret_cast<unsigned*>(parent + 8) = 0x0a0b0c0du;
    WriteEnd__9VIObjFileRUi(f, &size);
    if (log_.size() != 7 || log_[1] != "swap4" || log_[2] != "swap4" || log_[3] != "write8:67305985,218893066" || log_[4] != "swap4" || log_[5] != "swap4") return 10;
    return 0;
}
