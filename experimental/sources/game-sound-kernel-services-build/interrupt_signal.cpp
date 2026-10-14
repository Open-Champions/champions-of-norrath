#include "sound_kernel_services.hpp"
extern "C" NorrathSoundKernelRaw64
norrath_sound_interrupt_signal_semaphore(int semaphore) {
    return norrath_sound_kernel_services_detail::signal_service(semaphore);
}
