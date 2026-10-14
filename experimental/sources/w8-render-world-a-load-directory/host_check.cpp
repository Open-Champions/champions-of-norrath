#include <cstdio>
#include <cstring>
#include <vector>
extern "C" unsigned char CDmode[4];
unsigned char CDmode[4];
static std::vector<unsigned> read_sectors;
static std::vector<unsigned char> sectors[3];
extern "C" int sceCdRead(unsigned sector, unsigned count, void* buffer, void* mode) {
    read_sectors.push_back(sector);
    if (count != 1 || mode != CDmode) return 0;
    std::memcpy(buffer, sectors[sector - 100].data(), 2048);
    return 1;
}
static int syncs;
extern "C" int sceCdSync(int mode) { ++syncs; return mode; }
#include "load_directory.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static unsigned put_record(std::vector<unsigned char>& sector, unsigned position, const char* name, unsigned extent, unsigned size) {
    const unsigned length = 34 + static_cast<unsigned>(std::strlen(name));
    sector[position] = static_cast<unsigned char>(length);
    for (int i = 0; i < 4; ++i) {
        sector[position + 2 + i] = static_cast<unsigned char>(extent >> (8 * i));
        sector[position + 10 + i] = static_cast<unsigned char>(size >> (8 * i));
    }
    sector[position + 32] = static_cast<unsigned char>(std::strlen(name));
    std::memcpy(&sector[position + 33], name, std::strlen(name));
    return position + length;
}
int main() {
    for (auto& s : sectors) s.assign(2048, 0);
    unsigned p = put_record(sectors[0], 0, "ENGINE.LMP;1", 0x1111, 0x2222);
    put_record(sectors[0], p, "A", 0x3333, 0x4444);
    // sectors[1] stays empty: its first byte is zero.
    put_record(sectors[2], 0, "WORLD.LMP;1", 0x5555, 0x6666);
    static unsigned char directory[24580];
    std::memset(directory, 0xAB, sizeof directory);
    loadDirectory__FP9directoryii(directory, 100, 3);
    unsigned count;
    std::memcpy(&count, directory + 24576, 4);
    expect(count == 3, "three rows");
    expect(read_sectors.size() == 3 && read_sectors[0] == 100 && read_sectors[1] == 101 && read_sectors[2] == 102, "three consecutive sector reads");
    expect(syncs == 3, "one sync per read");
    expect(std::strcmp(reinterpret_cast<char*>(directory), "ENGINE.LMP;1") == 0, "first name");
    expect(std::strcmp(reinterpret_cast<char*>(directory + 32), "A") == 0, "second name");
    expect(std::strcmp(reinterpret_cast<char*>(directory + 64), "WORLD.LMP;1") == 0, "third name");
    unsigned word;
    std::memcpy(&word, directory + 16, 4); expect(word == 0x1111, "first extent at row +16");
    std::memcpy(&word, directory + 20, 4); expect(word == 0x2222, "first size at row +20");
    std::memcpy(&word, directory + 32 + 16, 4); expect(word == 0x3333, "second extent");
    std::memcpy(&word, directory + 32 + 20, 4); expect(word == 0x4444, "second size");
    std::memcpy(&word, directory + 64 + 16, 4); expect(word == 0x5555, "third extent");
    std::memcpy(&word, directory + 64 + 20, 4); expect(word == 0x6666, "third size");
    read_sectors.clear();
    loadDirectory__FP9directoryii(directory, 100, 0);
    std::memcpy(&count, directory + 24576, 4);
    expect(count == 0 && read_sectors.empty(), "zero sectors clears the count and reads nothing");
    return failures;
}
