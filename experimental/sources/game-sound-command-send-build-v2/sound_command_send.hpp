#pragma once
#include "sound_rpc_callbacks.hpp"
extern "C" {
// Private whole _sceSifSendCmd core: seven native input slots A0..T2.
// Both real DMA variants return the entire physical V0; no status is supplied.
unsigned long long norrath_sound_command_send_core(
    int command,int flags,void* packet,int packet_bytes,
    void* source,void* destination,int data_bytes);
unsigned long long norrath_sound_set_dma(void* descriptors,int count);
unsigned long long norrath_sound_interrupt_set_dma(void* descriptors,int count);
// Actual LOCAL _data_table +8 observation only: no storage/capacity/initializer.
extern unsigned char norrath_sound_command_dma_destination_word_view[];
}
namespace norrath_sound_command_send_detail {
using namespace norrath_sound_rpc_callbacks_detail;
using Raw64=unsigned long long;
static_assert(sizeof(Raw64)==8 && sizeof(int)==4 && sizeof(void*)==4,
              "reviewed EE raw V0 and target32 argument words");
}
