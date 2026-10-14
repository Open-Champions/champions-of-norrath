#pragma once
#include "sound_rpc_callbacks.hpp"
extern "C" {
// Genuine iSignalSema16, opcode -67. Existing SignalSema16 uses +66:
// they are separate original services, with no substituted provider.
unsigned long long norrath_sound_interrupt_signal_semaphore(int semaphore);
}
namespace norrath_sound_rpc_completion_detail {
using namespace norrath_sound_rpc_callbacks_detail;
using norrath_sound_native_register_detail::Raw64;
using norrath_sound_native_register_detail::current_gp_bits;
using norrath_sound_native_register_detail::install_gp_bits;
using norrath_sound_native_register_detail::signed_word_gp_bits;
// The actual original callback receives only client32 in A0. No guessed class
// or fabricated callback result is introduced; incidental V0 is unused.
using CompletionHandler=void (*)(void*);
inline CompletionHandler completion_handler(Address function_word) {
    return reinterpret_cast<CompletionHandler>(
        static_cast<__UINTPTR_TYPE__>(function_word));
}
}
