#include "sound_dma_chain.hpp"

extern "C" void norrath_sound_interrupt_set_dma_chain() {
    norrath_sound_dma_chain_detail::start_chain_service();
}
