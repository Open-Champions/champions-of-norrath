#define NORRATH_VSLOTS_B_HOST 1
#include "set_active_chunks.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int model_calls = 0;
static int chunk_calls = 0;
static void* seen_model = nullptr;
static int seen_which = 0;
static void* model_returns[2] = {nullptr, nullptr};
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void* model_stub(void* self) {
    (void)self;
    return model_returns[model_calls++];
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which) {
    ++chunk_calls;
    seen_model = model;
    seen_which = which;
    return 0x1122334455667788ull;
}
int main() {
    unsigned char object[600];
    unsigned char table[136];
    std::memset(object, 0x2a, sizeof object);
    std::memset(table, 0, sizeof table);
    vslots_b::write_u16(vslots_b::address(table) + 128u, 0u);
    vslots_b::write_word(vslots_b::address(table) + 132u, vslots_b::address_bits(reinterpret_cast<void*>(model_stub)));
    vslots_b::host_note(reinterpret_cast<void*>(model_stub));
    vslots_b::host_note(table);
    vslots_b::write_word(vslots_b::address(object) + 72u, vslots_b::address_bits(table));
    vslots_b::write_word(vslots_b::address(object) + 244u, 7u);
    vslots_b::write_u64(vslots_b::address(object) + 552u, 0x0102030405060708ull);
    model_returns[0] = nullptr;
    setActiveChunks__8Creature(object);
    expect(model_calls == 1 && chunk_calls == 0, "null model stops");
    expect(vslots_b::read_u64(vslots_b::address(object) + 552u) == 0x0102030405060708ull, "null model leaves 552");

    unsigned char model[4] = {1, 2, 3, 4};
    model_returns[0] = model;
    model_returns[1] = model + 1;
    model_calls = 0;
    setActiveChunks__8Creature(object);
    expect(model_calls == 2 && chunk_calls == 1, "a model is requested twice");
    expect(seen_model == model + 1, "the second model is passed on");
    expect(seen_which == 7, "object+244 selects the chunks");
    expect(vslots_b::read_u64(vslots_b::address(object) + 552u) == 0x1122334455667788ull, "eight return bytes land at 552");
    return failures ? 1 : 0;
}
