// Host check only; not a build input.
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> log_;
static int eofResults[4], eofIndex, readResult;
static long long tellResult;
extern "C" int Eof__6VIFile(unsigned char*) { return eofResults[eofIndex++]; }
extern "C" long long Tell__6VIFile(unsigned char*) { return tellResult; }
extern "C" int Read__6VIFilePvi(unsigned char* file, void* buffer, int count) {
    log_.push_back("read" + std::to_string(static_cast<unsigned char*>(buffer) - file) + "," + std::to_string(count));
    // type 0x0102, version 0x0304, size 0x0000002c, sub-object count 0
    static const unsigned char header[12] = {2, 1, 4, 3, 0x2c, 0, 0, 0, 0, 0, 0, 0};
    std::memcpy(buffer, header, 12);
    return readResult;
}
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    log_.push_back("swap" + std::to_string(size));
    for (int i = 0; i < size / 2; ++i) { unsigned char t = data[i]; data[i] = data[size - 1 - i]; data[size - 1 - i] = t; }
}
extern "C" { int VIEndian; }
#include "read_begin.cpp"
static void reset(unsigned char* f) {
    std::memset(f, 0, 700);
    *reinterpret_cast<int*>(f + 568) = 1;
    *reinterpret_cast<int*>(f + 232) = -1;
    std::memset(eofResults, 0, sizeof eofResults);
    eofIndex = 0; readResult = 12; tellResult = 0x100000010ll; log_.clear(); VIEndian = 1;
}
int main() {
    alignas(8) unsigned char f[700];
    unsigned short type = 0, version = 0;
    unsigned long long position = 0;
    unsigned size = 0;
    reset(f);
    *reinterpret_cast<int*>(f + 568) = 0;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || eofIndex != 0) return 1;
    reset(f);
    *reinterpret_cast<int*>(f + 572) = 1;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || eofIndex != 0) return 2;
    reset(f);
    *reinterpret_cast<int*>(f + 232) = 9;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || eofIndex != 0) return 3;
    reset(f);
    eofResults[0] = 1;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || eofIndex != 1 || !log_.empty()) return 4;
    // nested: the current entry must still have sub-objects and bytes left
    reset(f);
    *reinterpret_cast<int*>(f + 232) = 0;
    *reinterpret_cast<int*>(f + 248) = 0; *reinterpret_cast<int*>(f + 264) = 50;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || !log_.empty()) return 5;
    *reinterpret_cast<int*>(f + 248) = 1; *reinterpret_cast<int*>(f + 264) = 0;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || !log_.empty()) return 6;
    // top level, host byte order: header read into entry 0, index becomes 0
    reset(f);
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != 0) return 7;
    if (*reinterpret_cast<int*>(f + 232) != 0 || position != 0x100000010ull || size != 0x2c + 12) return 8;
    if (type != 0x0102 || version != 0x0304 || *reinterpret_cast<int*>(f + 264) != 0x2c) return 9;
    if (*reinterpret_cast<long long*>(f + 256) != 0x100000010ll || *reinterpret_cast<int*>(f + 576) != 1 || *reinterpret_cast<int*>(f + 584) != 1) return 10;
    if (log_.size() != 1 || log_[0] != "read240,12") return 11;
    // swapped order: four swaps after the read
    reset(f);
    VIEndian = 0;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != 0) return 12;
    if (log_.size() != 5 || log_[1] != "swap2" || log_[2] != "swap2" || log_[3] != "swap4" || log_[4] != "swap4") return 13;
    if (type != 0x0201 || version != 0x0403 || size != 0x2c000000u + 12) return 14;
    if (*reinterpret_cast<int*>(f + 584) != 1) return 15;
    // a failed header read still leaves the index incremented
    reset(f);
    readResult = -1;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || *reinterpret_cast<int*>(f + 232) != -1) return 16;
    // end of file after the header
    reset(f);
    eofResults[1] = 1;
    if (ReadBegin__9VIObjFileRUsT1RUlRUi(f, &type, &version, &position, &size) != -1 || *reinterpret_cast<int*>(f + 232) != 0 || *reinterpret_cast<int*>(f + 576) != 0) return 17;
    return 0;
}
