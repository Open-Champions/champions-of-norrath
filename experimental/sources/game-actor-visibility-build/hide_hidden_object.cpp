#include "actor_visibility.hpp"
extern "C" void norrath_actor_hide_hidden_object(void* object) {
    using namespace norrath_constructor_detail;
    const Address base = address(object);
    if ((read_word(base + 12u) & 0x20u) == 0u) return;
    if ((*bytes(base + 570u) & 0x08u) == 0u) return;
    norrath_actor_remove_from_grid(object);
}
