#include "sound_register_semaphore.hpp"
extern "C" int norrath_sound_delete_semaphore(int semaphore) {
    const auto result=norrath_sound_register_semaphore_detail::one_input_service(65,semaphore);
    return norrath_sound_register_semaphore_detail::signed_word(unsigned(result));
}
