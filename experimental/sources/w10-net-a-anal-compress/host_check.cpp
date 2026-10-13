// Host check for analCompress. Not an EE plan input. The expected words are
// derived by hand from the pass descriptions in the source header.
#include "anal_compress.cpp"
#include <cstring>
static int run(const unsigned char* data, int n, int index, int value, unsigned short* out, int* position,
               int* index_after, int* value_after) {
    BitWriter writer;
    std::memset(out, 0, 16);
    writer.data = out;
    writer.position = 0;
    AnalState state = {index, value};
    analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(0, &writer, &state,
                                                                          const_cast<unsigned char*>(data), n);
    *position = writer.position;
    *index_after = state.index;
    *value_after = state.value;
    return 0;
}
int main() {
    unsigned short out[8];
    int pos, idx, val;
    unsigned char a[3] = {128, 128, 128};
    run(a, 3, 0, 128, out, &pos, &idx, &val);  // three zero deltas: 5-bit count 3
    if (pos != 5 || out[0] != 0x1800 || idx != 0 || val != 128) return 1;
    unsigned char b[1] = {132};  // +1 step: 00000, sign 0, run 1
    run(b, 1, 0, 128, out, &pos, &idx, &val);
    if (pos != 9 || out[0] != 0x0080 || idx != 2 || val != 132) return 2;
    unsigned char c[1] = {120};  // -2 steps limited to one: 00000, sign 1, run 1
    run(c, 1, 0, 128, out, &pos, &idx, &val);
    if (pos != 9 || out[0] != 0x0480 || idx != 2 || val != 124) return 3;
    unsigned char d[32];  // 31 zero deltas then one: escape 11111, count 0, sign 0, run 1
    std::memset(d, 128, sizeof d);
    d[31] = 132;
    run(d, 32, 0, 128, out, &pos, &idx, &val);
    if (pos != 14 || out[0] != 0xf804 || val != 132) return 4;
    run(a, 0, 3, 77, out, &pos, &idx, &val);  // empty input writes nothing
    if (pos != 0 || out[0] != 0 || idx != 3 || val != 77) return 5;
    unsigned char e[1] = {255};  // large jump saturates: magnitude 1 moves value by one step only
    run(e, 1, 8, 0, out, &pos, &idx, &val);
    if (val != 64 || idx != 8) return 6;
    unsigned char f[1] = {0};  // downward jump: one step of 8 down, index up by two
    run(f, 1, 2, 20, out, &pos, &idx, &val);
    if (val != 12 || idx != 4) return 7;
    unsigned char g[1] = {100};
    run(g, 1, 1, 100, out, &pos, &idx, &val);  // small difference keeps the value and lowers the index
    if (val != 100 || idx != 0) return 8;
    return 0;
}
