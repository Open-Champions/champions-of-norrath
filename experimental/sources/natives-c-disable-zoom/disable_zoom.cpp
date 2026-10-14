// Observed DisableZooming__6Camera at 0x0026c100, 140 bytes. It saves the
// word at byte 84, forces bytes 80 and 84 to 1, writes the saved word at
// byte 88, and writes 1.0 at bytes 144 and 160. The distance words live at
// byte 176. The stored blend uses distance[1] against distance[0] and
// distance[2]. The word at byte 252 is distance[4] mixed with
// distance[old + 3] by min(1, 1).
namespace {
float load_float(const unsigned char* bytes, unsigned offset) {
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[offset];
    out[1] = bytes[offset + 1];
    out[2] = bytes[offset + 2];
    out[3] = bytes[offset + 3];
    return value;
}

void store_float(unsigned char* bytes, unsigned offset, float value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[offset] = in[0];
    bytes[offset + 1] = in[1];
    bytes[offset + 2] = in[2];
    bytes[offset + 3] = in[3];
}

int load_int(const unsigned char* bytes, unsigned offset) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[offset];
    out[1] = bytes[offset + 1];
    out[2] = bytes[offset + 2];
    out[3] = bytes[offset + 3];
    return value;
}

void store_int(unsigned char* bytes, unsigned offset, int value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[offset] = in[0];
    bytes[offset + 1] = in[1];
    bytes[offset + 2] = in[2];
    bytes[offset + 3] = in[3];
}

float one() {
    const unsigned bits = 0x3f800000u;
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
}

extern "C" void DisableZooming__6Camera(void* camera) {
    auto* bytes = static_cast<unsigned char*>(camera);
    const int old_level = load_int(bytes, 84);
    const float unit = one();
    const float scale = unit < unit ? unit : unit;
    const unsigned old_offset = 176u + (static_cast<unsigned>(old_level) << 2);
    const float distance_one = load_float(bytes, 180);
    store_float(bytes, 144, unit);
    store_float(bytes, 160, unit);
    const float distance_zero = load_float(bytes, 176);
    const float selected = load_float(bytes, old_offset);
    const float distance_two = load_float(bytes, 184);
    const float far_word = load_float(bytes, 192);
    const float span = distance_two - distance_zero;
    store_int(bytes, 84, 1);
    store_int(bytes, 80, 1);
    store_int(bytes, 88, old_level);
    const float blended = selected + (distance_one - selected);
    store_float(bytes, 140, blended);
    const float ratio = (blended - distance_zero) / span;
    store_float(bytes, 216, unit - ratio);
    const float sample = load_float(bytes, old_offset + 12u);
    const float mixed = sample + (far_word - sample) * scale;
    store_float(bytes, 252, mixed);
}
