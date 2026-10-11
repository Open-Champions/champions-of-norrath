#pragma once
#include "load_entities.hpp"
#include "constructor.hpp"
#include "add_node.hpp"
static_assert(sizeof(unsigned long long) == 8 && sizeof(long long) == 8,
              "AMX boundary transports the lower 64-bit register values");
extern "C" {
// Unsized observations of indexed original words. No owner or payload is defined.
extern unsigned char norrath_script_current_amx_word_view[];
extern unsigned char norrath_script_complain_frame_word_view[];
// Genuine unresolved original callees. Their outputs are produced by those
// callees, never by a host response or a locally substituted success value.
unsigned long long norrath_amx_find_public(void* amx, const char* name,
                                         int* public_index);
unsigned long long norrath_amx_exec(void* amx, int* result,
                                  long long public_index,
                                  long long parameter_count,
                                  const void* parameter_data);
}
