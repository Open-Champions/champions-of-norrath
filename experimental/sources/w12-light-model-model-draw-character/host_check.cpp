// Host contract for modelDrawCharacter__FP6PacketP12_modelHeaderli. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* last; };
extern "C" void modelDrawCharacter__FP6PacketP12_modelHeaderli(Packet*, const unsigned char*, long long, int);
static unsigned long long tagAt(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[512];
    alignas(16) unsigned char out[256];
    auto build = [&](int count, int parts, const unsigned* rows, int words) {
        std::memset(model, 0, sizeof model);
        model[38] = static_cast<unsigned char>(count);
        model[18] = static_cast<unsigned char>(parts);
        std::memcpy(model + 40, rows, words * 4);
    };
    // one row, three parts: spans 0x1000..0x1100, ..0x1300, ..0x1500, then the end word
    const unsigned rowA[] = {0x1000, 0x1100, 0x1300, 0x1500, 0x1600};
    build(1, 3, rowA, 5);
    Packet p{out, nullptr};
    std::memset(out, 0xaa, sizeof out);
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x6, 0);
    expect(p.cursor == out + 16 && p.last == out, "mask 6: one tag");
    expect(tagAt(out) == (0x40ull | 0x30000000ull | (0x1100ull << 32)), "mask 6: tag value");
    expect(tagAt(out + 8) == 0, "mask 6: high doubleword");
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x1, 0);
    expect(p.cursor == out + 16 && tagAt(out) == (0x10ull | 0x30000000ull | (0x1000ull << 32)), "mask 1: first span");
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x0, 0);
    expect(p.cursor == out && p.last == nullptr, "mask 0: nothing");
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x5, 0);
    // bit 0 set -> [0x1000,0x1100) flushed at i=1; bit 2 set -> [0x1300? start=row[2]) flushed at 3
    expect(p.cursor == out + 32, "mask 5: two tags");
    expect(tagAt(out) == (0x10ull | 0x30000000ull | (0x1000ull << 32)), "mask 5: first");
    expect(tagAt(out + 16) == (0x20ull | 0x30000000ull | (0x1300ull << 32)), "mask 5: second");
    // bit p of the mask is ignored
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x8, 0);
    expect(p.cursor == out, "mask bit p ignored");
    // negative part count returns without writing
    model[18] = 0xff;
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, -1, 0);
    expect(p.cursor == out, "negative parts");
    // two rows: the index selects the row and is clamped
    const unsigned rowB[] = {0x1000, 0x1100, 0x1200, 0x2000, 0x2400, 0x2800, 0x2c00};
    build(2, 2, rowB, 7);
    // row stride is 12 bytes; the layout above holds row 1 at words 3..5
    for (int idx : {1, 7}) {
        p = Packet{out, nullptr};
        modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x1, idx);
        expect(p.cursor == out + 16 && tagAt(out) == (0x40ull | 0x30000000ull | (0x2000ull << 32)), "row 1 span");
    }
    p = Packet{out, nullptr};
    modelDrawCharacter__FP6PacketP12_modelHeaderli(&p, model, 0x1, -5);
    expect(p.cursor == out + 16 && tagAt(out) == (0x10ull | 0x30000000ull | (0x1000ull << 32)), "negative index clamps to row 0");
    if (failures) return 1;
    std::printf("modelDrawCharacter contract passed\n");
    return 0;
}
