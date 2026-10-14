#include "sound_memory_lifecycle.hpp"
using namespace norrath_sound_memory_lifecycle_detail;

extern "C" void norrath_player_free_plain(void* allocation) {
    const Address impure=address(norrath_factory_impure_pointer_word);
    norrath_player_malloc_lock(bytes(read_word_fresh(impure)));
    // Both rereads are mandatory even if the intervening real service changes
    // the global reentrancy pointer. Null allocation still takes all three calls.
    norrath_player_free_reentrant(bytes(read_word_fresh(impure)),
                                  static_cast<unsigned char*>(allocation));
    norrath_player_malloc_unlock(bytes(read_word_fresh(impure)));
}

