#include "save_level_data_async.cpp"
#include <cstdio>
#include <cstring>
#include <string>

int norrath_map_auto_map_bits_size __asm__("_3map$autoMapBitsSize") = 50;
static std::string log_;
static int objRet, floorRet;
static char* base;
extern "C" {
void gameGetFullLevelName__FPc(char* b) { log_ += "n;"; b[0] = 0; }
int objectGetSaveData__FPci(char* d, int room) { log_ += "o" + std::to_string(d - base) + "," + std::to_string(room) + ";"; return objRet; }
int floorGetSaveInfo__FPci(char* d, int room) { log_ += "f" + std::to_string(d - base) + "," + std::to_string(room) + ";"; return floorRet; }
void writeMap__3mapPUc(unsigned char* d) { log_ += "w" + std::to_string(reinterpret_cast<char*>(d) - base) + ";"; }
}

static int fails;
static void expect(bool ok, const char* what) { if (!ok) { ++fails; std::printf("FAIL %s\n", what); } }

int main() {
    alignas(8) char buf[512];
    std::memset(buf, 0xee, sizeof buf);
    base = buf + 1;  // misaligned destination exercises the unaligned stores
    objRet = 100; floorRet = 36;
    saveLevelDataASync__FPci(base, 1000);
    // room = 1000 - 50 - 24 = 926; object data at +8; floor info at +100+16 with room 826+8; map at +100+8+36+8
    expect(log_ == "n;o8,926;f116,834;w152;", log_.c_str());
    int a, b;
    std::memcpy(&a, base, 4);
    std::memcpy(&b, base + 108, 4);
    expect(a == 100, "word A at +0");
    expect(b == 36, "word B at +A+8");
    expect(static_cast<unsigned char>(base[4]) == 0xee && static_cast<unsigned char>(base[104]) == 0xee,
           "no other destination bytes written");
    log_.clear();
    objRet = 0; floorRet = 0;
    saveLevelDataASync__FPci(base, 24 + 50);
    expect(log_ == "n;o8,0;f16,8;w16;", log_.c_str());
    std::printf(fails ? "FAILED\n" : "ok: layout, offsets and room arithmetic\n");
    return fails;
}
