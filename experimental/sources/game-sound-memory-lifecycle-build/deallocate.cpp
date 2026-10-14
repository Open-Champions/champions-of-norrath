#include "sound_memory_lifecycle.hpp"

// The unique generated private name denotes the entire genuine original WEAK
// __builtin_delete32. It does not assert an original GLOBAL identity or alias.
extern "C" void norrath_animation_state_deallocate(void* allocation) {
    if(allocation!=nullptr)norrath_mem_free_wrapper(allocation);
}

