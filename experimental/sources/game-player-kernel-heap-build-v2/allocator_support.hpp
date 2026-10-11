#pragma once
#include "reentrant_malloc.hpp"
extern "C" {
NorrathFormatUnsigned64 norrath_player_adjust_break_reentrant(unsigned char*,int);
NorrathFormatUnsigned64 norrath_player_adjust_break(int);
NorrathFormatUnsigned64 norrath_player_thread_id_bits();
int norrath_player_wait_semaphore(int);
int norrath_player_signal_semaphore(int);
extern unsigned char norrath_player_malloc_lock_owner_view[];
extern unsigned char norrath_player_malloc_lock_count_view[];
extern unsigned char norrath_player_malloc_semaphore_view[];
extern unsigned char norrath_player_malloc_break_base_view[];
extern unsigned char norrath_player_malloc_max_break_view[];
extern unsigned char norrath_player_malloc_max_total_view[];
extern unsigned char norrath_player_malloc_current_info_view[];
extern unsigned char norrath_player_syscall_errno_word[];
}
namespace norrath_allocator_support_detail {
using namespace norrath_malloc_detail;
inline NorrathFormatSigned64 signed_wide(Wide bits) {
    return bits<=0x7fffffffffffffffull?NorrathFormatSigned64(bits):-1-NorrathFormatSigned64(~bits);
}
}
