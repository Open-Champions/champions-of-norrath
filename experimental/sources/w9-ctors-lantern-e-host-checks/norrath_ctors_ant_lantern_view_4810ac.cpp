// Host comparison only. Not an EE build input.
extern "C" const float norrath_ctors_ant_lantern_view_4810ac;
int main() {
    unsigned bits = 0u;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&norrath_ctors_ant_lantern_view_4810ac);
    for (unsigned i = 0; i < 4u; ++i) bits |= unsigned(bytes[i]) << (8u * i);
    return bits == 0x3f8cccccu ? 0 : 1;
}
