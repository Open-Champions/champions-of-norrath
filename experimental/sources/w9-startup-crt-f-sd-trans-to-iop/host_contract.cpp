// Host check only; not an EE build input. Pointers are 8 bytes on the host,
// so the record layout is checked by field rather than by size.
struct SifDmaData {
    void* source;
    unsigned destination;
    unsigned size;
    unsigned attribute;
};
extern "C" {
static int set_calls, stat_calls, set_result, stat_busy;
static SifDmaData seen;
static int seen_count;
int sceSifSetDma(SifDmaData* data, int count) { ++set_calls; seen = *data; seen_count = count; return set_result; }
int sceSifDmaStat(int id) { ++stat_calls; return stat_calls <= stat_busy ? 0 : (id == 5 ? -1 : -2); }
int sceSdTransToIOP(void* source, unsigned destination, unsigned size, int mode);
}
int main() {
    int x;
    if (sceSdTransToIOP(&x, 0x1000, 64, 0) != -1 || set_calls != 0) return 1;
    set_result = 0;
    if (sceSdTransToIOP(&x, 0x1000, 64, 1) != -1 || set_calls != 1 || stat_calls != 0) return 2;
    set_result = 5; stat_busy = 3;
    if (sceSdTransToIOP(&x, 0x1000, 64, 1) != 0 || stat_calls != 4) return 3;
    if (seen.source != &x || seen.destination != 0x1000 || seen.size != 64 || seen.attribute != 0 || seen_count != 1) return 4;
    stat_calls = 0;
    if (sceSdTransToIOP(&x, 0x2000, 32, 2) != 0 || stat_calls != 0) return 5;   // other modes do not wait
    return 0;
}
