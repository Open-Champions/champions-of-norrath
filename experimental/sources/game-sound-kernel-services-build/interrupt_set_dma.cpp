#include "sound_kernel_services.hpp"
extern "C" NorrathSoundKernelRaw64
norrath_sound_interrupt_set_dma(void* descriptors,int count) {
    return norrath_sound_kernel_services_detail::dma_service(-119,descriptors,count);
}
