// Host comparison only. Not an EE build input.
extern "C" const float norrath_player_view_47fed4;
int main() {
    unsigned bits = 0u;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&norrath_player_view_47fed4);
    for (unsigned i = 0; i < 4u; ++i) bits |= unsigned(bytes[i]) << (8u * i);
    return bits == 0x38490fdau ? 0 : 1;
}
