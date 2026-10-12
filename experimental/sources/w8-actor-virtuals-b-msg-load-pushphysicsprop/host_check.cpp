#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "msg_load.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char object[320];
static std::vector<std::string> log_calls;
static std::vector<int> pool_args;
static std::vector<std::pair<unsigned, unsigned>> lump_args;
static float body_args[5];
static void* body_ptr;
static void* update_ptr;
static float f_at_first_call = -1.0f;
static unsigned bits_96_at_init;

extern "C" unsigned objectSaveGetFromStringPool__Fi(int index) {
    if (log_calls.empty()) std::memcpy(&f_at_first_call, object + 256, 4);
    log_calls.push_back("pool");
    pool_args.push_back(index);
    return 0x2000u + static_cast<unsigned>(index);
}
extern "C" unsigned lumpFindResource__FPcT0(unsigned name, unsigned kind) {
    log_calls.push_back("lump");
    lump_args.push_back({name, kind});
    return 0x9000u + static_cast<unsigned>(lump_args.size());
}
extern "C" void bodyInit__FP5_bodyfffff(void* body, float a, float b, float c, float d, float e) {
    log_calls.push_back("init");
    body_ptr = body;
    body_args[0] = a; body_args[1] = b; body_args[2] = c; body_args[3] = d; body_args[4] = e;
    std::memcpy(&bits_96_at_init, object + 96, 4);
}
extern "C" void bodyUpdateMatrix__FP5_body(void* body) {
    log_calls.push_back("update");
    update_ptr = body;
}

int main() {
    alignas(8) unsigned char stream[96];
    std::memset(stream, 0, sizeof stream);
    std::memset(object, 0x31, sizeof object);
    float f = 2.5f;
    std::memcpy(stream, &f, 4);
    int words[3] = {5, 6, 7};
    std::memcpy(stream + 4, words, sizeof words);
    for (unsigned i = 0; i < 13u; ++i) {
        float v = 100.0f + static_cast<float>(i);
        std::memcpy(stream + 16 + 4 * i, &v, 4);
    }
    int used = msg_load__15PushPhysicsPropPc(object, reinterpret_cast<const char*>(stream));
    expect(used == 68, "consumes 68 bytes");
    expect(f_at_first_call == 2.5f, "the float is stored before the first pool call");
    expect(pool_args == std::vector<int>({5, 6, 7}), "pool lookups in order");
    const std::vector<std::string> want = {"pool", "pool", "pool", "lump", "lump", "init", "update"};
    expect(log_calls == want, "call order");
    unsigned s268, s272, s276, s260, s264;
    std::memcpy(&s268, object + 268, 4); std::memcpy(&s272, object + 272, 4); std::memcpy(&s276, object + 276, 4);
    std::memcpy(&s260, object + 260, 4); std::memcpy(&s264, object + 264, 4);
    expect(s268 == 0x2005u && s272 == 0x2006u && s276 == 0x2007u, "pool results at +268/+272/+276");
    expect(lump_args.size() == 2u && lump_args[0] == std::make_pair(0x2005u, 0x2006u) &&
               lump_args[1] == std::make_pair(0x2005u, 0x2007u),
           "lump arguments");
    expect(s260 == 0x9001u && s264 == 0x9002u, "lump results at +260 and +264");
    float f256, f32, f28;
    std::memcpy(&f256, object + 256, 4); std::memcpy(&f32, object + 32, 4); std::memcpy(&f28, object + 28, 4);
    expect(f256 == 2.5f && f32 == 5.0f, "f and 2f");
    expect(f28 > 2.9999f && f28 < 3.0001f, "f times 1.2");
    unsigned f28bits;
    std::memcpy(&f28bits, &f28, 4);
    float expected = 2.5f * bits_to_float(0x3f999999u);
    unsigned expected_bits;
    std::memcpy(&expected_bits, &expected, 4);
    expect(f28bits == expected_bits, "the constant is the exact float 0x3f999999");
    expect(body_ptr == object + 80 && update_ptr == object + 80, "body at +80");
    expect(body_args[0] == 1.0f && body_args[1] == 1.0f && body_args[2] == 5.0f && body_args[3] == 5.0f && body_args[4] == 5.0f,
           "bodyInit arguments");
    expect(bits_96_at_init == 0x31313131u, "the +96 floats are copied after bodyInit");
    bool floats = true;
    for (unsigned i = 0; i < 13u; ++i) {
        float v;
        std::memcpy(&v, object + 96 + 4 * i, 4);
        if (v != 100.0f + static_cast<float>(i)) floats = false;
    }
    expect(floats, "13 floats at +96..+144");
    expect(object[95] == 0x31 && object[148] == 0x31, "neighbours untouched");
    return failures ? 1 : 0;
}
