// Host comparison only. Not an EE build input.
extern "C" const float norrath_ctors_ant_lantern_view_481abc;
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_481abc != 4u) return 1;
    const float value = norrath_ctors_ant_lantern_view_481abc;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    unsigned bits = static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
                    (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
    return bits == 0x3e4cccccu ? 0 : 1;
}
