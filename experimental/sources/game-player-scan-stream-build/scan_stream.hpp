#pragma once
#include "stream_io.hpp"
extern "C" {
int norrath_player_stream_refill(unsigned char*);
unsigned norrath_player_stream_read_elements(void*,unsigned,unsigned,unsigned char*);
int norrath_player_stream_unget(int,unsigned char*);
const char* norrath_player_scan_character_class(unsigned char*,const char*);
void norrath_player_free_reentrant(unsigned char*,unsigned char*);
void norrath_player_stream_make_buffer(unsigned char*);
int norrath_player_stream_extend_pushback(unsigned char*);
void* norrath_copy_bytes(void*,const void*,__SIZE_TYPE__);
}
