// Host comparison only. Not an EE build input.
extern "C" const float norrath_actors_b_view_4816d4;

int main() {
    unsigned bits = 0;
    const float value = norrath_actors_b_view_4816d4;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    bits = static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
    return bits == 0x461c4000u && value == 10000.0f ? 0 : 1;
}
