#include <cstdio>
#include <cstring>
#include <vector>

#include "msg_load.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<int> pool_args;
static std::vector<std::pair<unsigned, unsigned>> lump_args;
static std::vector<int> order;  // 1 pool, 2 lump

extern "C" unsigned objectSaveGetFromStringPool__Fi(int index) {
    pool_args.push_back(index);
    order.push_back(1);
    return 0x1000u + static_cast<unsigned>(index) * 16u;
}
extern "C" unsigned lumpFindResource__FPcT0(unsigned name, unsigned kind) {
    lump_args.push_back({name, kind});
    order.push_back(2);
    return 0x70000u + static_cast<unsigned>(lump_args.size());
}

static void run(unsigned options, bool expect_flag) {
    alignas(8) unsigned char stream[40];
    alignas(8) unsigned char object[128];
    std::memset(stream, 0x5e, sizeof stream);
    std::memset(object, 0x21, sizeof object);
    const int words[5] = {3, 4, 5, 6, 7};
    std::memcpy(stream, words, sizeof words);
    const short halves[2] = {-300, 1200};
    std::memcpy(stream + 20, halves, sizeof halves);
    std::memcpy(stream + 24, &options, sizeof options);
    *reinterpret_cast<unsigned*>(object + 12) = 0x00000041u;
    pool_args.clear();
    lump_args.clear();
    order.clear();
    int used = msg_load__9ContainerPc(object, reinterpret_cast<const char*>(stream));
    expect(used == 28, "consumes 28 bytes");
    expect(pool_args == std::vector<int>({3, 4, 5, 6, 7}), "pool lookups in stream order");
    const unsigned* out = reinterpret_cast<const unsigned*>(object + 76);
    expect(out[0] == 0x1030u && out[1] == 0x1040u && out[2] == 0x1050u && out[3] == 0x1060u && out[4] == 0x1070u,
           "pool results at +76..+92");
    expect(*reinterpret_cast<float*>(object + 28) == -300.0f, "first half as float at +28");
    expect(*reinterpret_cast<float*>(object + 32) == 1200.0f, "second half as float at +32");
    expect(*reinterpret_cast<unsigned*>(object + 104) == options, "options word at +104");
    expect(lump_args.size() == 2u && lump_args[0].first == 0x1030u && lump_args[0].second == 0x1040u &&
               lump_args[1].first == 0x1030u && lump_args[1].second == 0x1050u,
           "lump lookups use +76 with +80 then +84");
    expect(*reinterpret_cast<unsigned*>(object + 96) == 0x70001u && *reinterpret_cast<unsigned*>(object + 100) == 0x70002u,
           "lump results at +96 and +100");
    expect(order == std::vector<int>({1, 1, 1, 1, 1, 2, 2}), "five pool calls, then two lump calls");
    expect(*reinterpret_cast<unsigned*>(object + 12) == (expect_flag ? 0x00100041u : 0x00000041u),
           "bit 20 follows bit 2 of the options");
    expect(object[0] == 0x21 && object[75] == 0x21 && object[108] == 0x21, "other bytes untouched");
}

int main() {
    run(4, true);
    run(0xfffffffbu, false);
    run(0xffffffffu, true);
    run(0, false);
    return failures ? 1 : 0;
}
