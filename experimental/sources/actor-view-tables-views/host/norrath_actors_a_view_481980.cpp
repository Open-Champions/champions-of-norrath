// Host comparison only. Not an EE build input.
extern "C" const float norrath_actors_a_view_481980;
int main() {
    const float value = norrath_actors_a_view_481980;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    unsigned bits = static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
                    (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
    return bits == 0x3791a2b3u ? 0 : 1;
}
