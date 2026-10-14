#include "sound_register_semaphore.hpp"
extern "C" void norrath_sound_set_register(int register_id,unsigned value) {
    // The genuine physical kernel result is modeled, then ignored by the
    // existing effect-only source API. Incidental original V0 is not promised.
    (void)norrath_sound_register_semaphore_detail::set_register_service(register_id,value);
}
