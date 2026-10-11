#pragma once
#include "scan_stream.hpp"
extern "C" {
unsigned char* norrath_player_malloc_reentrant(unsigned char*,unsigned);
unsigned char* norrath_player_reallocate_reentrant(unsigned char*,unsigned char*,unsigned);
int norrath_player_file_status_reentrant(unsigned char*,int,unsigned char*);
int norrath_player_descriptor_is_terminal(int);
int norrath_player_file_status(int,unsigned char*);
extern unsigned char norrath_player_syscall_errno_word[]; // Actual errno4 at486df0; no owner defined.
}
