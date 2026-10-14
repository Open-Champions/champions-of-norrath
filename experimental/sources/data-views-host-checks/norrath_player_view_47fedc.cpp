// Host comparison only. Not an EE build input.
extern "C" const float norrath_player_view_47fedc;
int main() {
    unsigned bits = 0u;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&norrath_player_view_47fedc);
    for (unsigned i = 0; i < 4u; ++i) bits |= unsigned(bytes[i]) << (8u * i);
    return bits == 0x3c086b24u ? 0 : 1;
}
