// Host check for norrath_w13_spell_msg_a_view_7d6a10. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "norrath_w13_spell_msg_a_view_7d6a10.cpp"
int main() {
    if (sizeof norrath_w13_spell_msg_a_view_7d6a10 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&norrath_w13_spell_msg_a_view_7d6a10) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof norrath_w13_spell_msg_a_view_7d6a10; ++i)
        if (norrath_w13_spell_msg_a_view_7d6a10[i] != 0) return 3;
    return 0;
}
