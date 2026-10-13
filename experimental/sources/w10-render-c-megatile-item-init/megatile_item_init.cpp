// Observed megatileItemInit__FP8MegatilePv at 0x00266100, 40 bytes. Arguments: a
// megatile record and a base value. No result. The base is added, as a 32-bit
// value, to three words of the record: the words at +0, +8 and +200. Inferred: a
// relocation of stored offsets to addresses after load. Unknown: what those three
// fields hold.
struct Megatile;

extern "C" void megatileItemInit__FP8MegatilePv(Megatile* tile, void* base) {
    unsigned* words = reinterpret_cast<unsigned*>(tile);
    unsigned b = static_cast<unsigned>(reinterpret_cast<unsigned long>(base));
    words[0] = words[0] + b;
    words[2] = words[2] + b;
    words[50] = words[50] + b;
}
