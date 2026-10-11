// Host comparison only. Not an EE build input.
extern "C" const float norrath_creature_view_47fb68;
int main() {
    unsigned bits = 0u;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&norrath_creature_view_47fb68);
    for (unsigned i = 0; i < 4u; ++i) bits |= unsigned(bytes[i]) << (8u * i);
    return bits == 0x3fa9374bu ? 0 : 1;
}
