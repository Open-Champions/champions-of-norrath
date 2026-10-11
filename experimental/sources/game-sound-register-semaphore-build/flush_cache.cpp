#include "sound_register_semaphore.hpp"
extern "C" void norrath_sound_flush_cache(int mode) {
    (void)norrath_sound_register_semaphore_detail::one_input_service(100,mode);
}
