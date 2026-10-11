#include "sound_register_semaphore.hpp"
extern "C" int norrath_sound_create_semaphore(void* parameters) {
    const auto result=norrath_sound_register_semaphore_detail::one_input_service(64,parameters);
    // Existing canonical int API promises signed low32 only. The actual native
    // syscall wrapper returns full V0; no upper-result or kernel-answer proof.
    return norrath_sound_register_semaphore_detail::signed_word(unsigned(result));
}
