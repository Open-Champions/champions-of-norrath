// Observed SetDistanceAtLevel__6Cameraif at 0x0026fba0, 132 bytes.
// The distance argument is always stored at byte 176 + level*4. The rest
// runs only when level equals the word at byte 84. That path rewrites the
// words at bytes 140, 216, and 252 from the distance table, the word at
// byte 88, and min(1, the word at byte 144).
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

extern "C" void SetDistanceAtLevel__6Cameraif(void* camera, int level, float distance) {
    auto* bytes = static_cast<unsigned char*>(camera);
    const unsigned slot = 176u + (static_cast<unsigned>(level) << 2);
    const int current = load_int(bytes, 84);
    store_float(bytes, slot, distance);
    if (level != current) return;
    const float unit = one();
    const float distance_zero = load_float(bytes, 176);
    const float distance_two = load_float(bytes, 184);
    const float span = distance_two - distance_zero;
    const float cap = load_float(bytes, 144);
    const float limited = unit < cap ? unit : cap;
    const int saved = load_int(bytes, 88);
    store_float(bytes, 140, distance);
    const float ratio = (distance - distance_zero) / span;
    store_float(bytes, 216, unit - ratio);
    const unsigned saved_sample = 188u + (static_cast<unsigned>(saved) << 2);
    const unsigned level_sample = 188u + (static_cast<unsigned>(level) << 2);
    const float from_saved = load_float(bytes, saved_sample);
    const float from_level = load_float(bytes, level_sample);
    store_float(bytes, 252, from_saved + (from_level - from_saved) * limited);
}
