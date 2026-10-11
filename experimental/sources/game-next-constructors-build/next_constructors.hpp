#pragma once
#include "constructor.hpp"
static_assert(sizeof(short)==2,"actual low16 field transport");
extern "C" {
// Exact frozen factory interfaces; integer Lever type differs from Trap short.
unsigned norrath_construct_TriggerParams(void*, const void*, char**);
unsigned norrath_construct_PokeReflector(void*, const void*, char**);
unsigned norrath_construct_Lever(void*, const void*, int, char**);
unsigned norrath_construct_Timer(void*, const void*, char**);
unsigned norrath_construct_Counter(void*, const void*, char**);
unsigned norrath_construct_Trap(void*, const void*, short, char**);
// Existing complete genuine slow wrapper; real list removal/link dependencies.
unsigned norrath_actor_add_slow_run_list(void*);
// Actual address identities only, with no emitted data payload or owner extent.
extern unsigned char norrath_next_view_44c958[];
extern unsigned char norrath_next_view_449280[];
extern unsigned char norrath_next_view_4491f0[];
extern unsigned char norrath_next_view_4490d0[];
extern unsigned char norrath_next_view_449160[];
extern unsigned char norrath_next_view_44e820[];
extern unsigned char norrath_next_view_42c650[];
}
