#include "actor_visibility.hpp"
extern "C" void norrath_actor_remove_monster(void* object) {
    using namespace norrath_constructor_detail;
    const Address base = address(object);
    const unsigned object_flags = read_word(base + 12u);
    if ((object_flags & 0x20u) == 0u) return;
    const int state = signed_byte(*bytes(base + 570u));
    // The original's branch delay reads this byte again before either exit.
    const unsigned char fresh_state = *bytes(base + 570u);
    if (state != 0) return;
    *bytes(base + 570u) = static_cast<unsigned char>(fresh_state | 0x04u);
    // This original store occupies the grid-removal call delay and must be
    // visible before the callee reads the object. Use captured object flags.
    write_word(base + 12u, object_flags | 0x00088000u);
    norrath_actor_remove_from_grid(object);
}
