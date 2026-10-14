// Host check for norrath_w13_spell_msg_a_view_8d6370. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "norrath_w13_spell_msg_a_view_8d6370.cpp"
int main() {
    if (sizeof norrath_w13_spell_msg_a_view_8d6370 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&norrath_w13_spell_msg_a_view_8d6370) % 16u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&norrath_w13_spell_msg_a_view_8d6370);
    for (unsigned i = 0; i < sizeof norrath_w13_spell_msg_a_view_8d6370; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
