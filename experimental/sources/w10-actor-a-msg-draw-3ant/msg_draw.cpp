// Observed msg_draw__3Ant at 0x00208820, 76 bytes.
// Draws through the Creature draw routine. The signed byte at object+960 selects the third
// argument: with it zero, the word at m+20 is passed, otherwise the word at m+28, where m is the
// pointer stored at object+36. The second argument is the word at m+12 and the fourth argument is
// zero. When the byte is zero the call is a tail call. The result of the draw call is returned.
// Inferred: m is a resource block and the two words are alternate textures. Unknown: what the
// byte at +960 means. Nothing here ran on the EE.
extern "C" {
int draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
}

#if defined(W10_ACTOR_A_HOST)
extern unsigned char* w10_actor_a_host_base;  // host builds keep 32-bit words as offsets from this base
#endif
namespace {
unsigned char* from_word(unsigned word) {
#if defined(W10_ACTOR_A_HOST)
    return w10_actor_a_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
}  // namespace

extern "C" int msg_draw__3Ant(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* m = from_word(*reinterpret_cast<unsigned*>(object + 36));
    void* model = from_word(*reinterpret_cast<unsigned*>(m + 12));
    const unsigned textureOffset = static_cast<signed char>(object[960]) == 0 ? 20 : 28;
    void* texture = from_word(*reinterpret_cast<unsigned*>(m + textureOffset));
    return draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
}
