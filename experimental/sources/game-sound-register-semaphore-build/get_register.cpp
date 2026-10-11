#include "sound_register_semaphore.hpp"
extern "C" unsigned long long norrath_sound_get_register(int register_id) {
    return norrath_sound_register_semaphore_detail::one_input_service(122,register_id);
}
