#pragma once
#include "core_formatter.hpp"
extern "C" {
void norrath_player_free_reentrant(unsigned char*,unsigned char*);
void norrath_player_malloc_lock(unsigned char*);
void norrath_player_malloc_unlock(unsigned char*);
int norrath_player_malloc_trim_reentrant(unsigned char*,unsigned);
extern unsigned char norrath_player_malloc_bins_view[];
extern unsigned char norrath_player_malloc_trim_threshold_view[];
extern unsigned char norrath_player_malloc_top_pad_view[];
}
