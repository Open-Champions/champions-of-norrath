#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(short) == 2,
              "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 && sizeof(unsigned long long) == 8 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "native EE target representation");
extern "C" {
// Full genuine factory ABI: object A0, raw12-byte point-storage A1, signed16 A2, arguments A3.
unsigned norrath_construct_Cat(void*, const void*, short, char**);
unsigned norrath_construct_CdEyeball(void*, const void*, short, char**);
unsigned norrath_construct_Mermaid(void*, const void*, short, char**);
unsigned norrath_construct_SmallSpider(void*, const void*, short, char**);
unsigned norrath_construct_GnomeNavigator(void*, const void*, short, char**);
unsigned norrath_construct_Natasla(void*, const void*, short, char**);
// Complete genuine Creature parent; resource-map A3, arguments T0, float F12.
unsigned norrath_creature_construct(void*, const void*, short, void*, float, char**);
unsigned norrath_actor_random(void*);
void norrath_actor_animation_initialize(void*);
unsigned long long norrath_actor_model_active_chunks(void*, int);
unsigned norrath_actor_add_run_list(void*);
unsigned norrath_actor_add_slow_run_list(void*);
// Genuine existing complete GameObject/list/tag source interfaces.
unsigned norrath_game_object_construct(void*, const void*, void*, char**);
int norrath_game_object_find_tag_int(void*, char*, int);
void* norrath_list_oneway_link(void*, void*, void*);
// Genuine remaining effect-bearing dependencies, not locally supplied answers.
char* norrath_actor_find_tag_string(void*, char*, int);
void norrath_actor_remove_from_grid(void*);
unsigned norrath_actor_set_parameters(void*, float, float, float, float, float, int);
unsigned norrath_actor_finish_construction(void*);
void norrath_actor_animation_add(void*, void*, int);
void norrath_actor_animation_randomize_time(void*);
// Unsized mapped views. Hex names identify actual addresses, not guessed owners.
extern unsigned char norrath_actor_view_42b118[];
extern unsigned char norrath_actor_view_42c710[];
extern unsigned char norrath_actor_view_42b768[];
extern unsigned char norrath_actor_view_42c130[];
extern unsigned char norrath_actor_view_42baa8[];
extern unsigned char norrath_actor_view_42d0c0[];
extern unsigned char norrath_actor_view_4488f0[];
extern unsigned char norrath_actor_view_44c170[];
extern unsigned char norrath_actor_view_44b170[];
extern unsigned char norrath_actor_view_44e1c0[];
extern unsigned char norrath_actor_view_44b540[];
extern unsigned char norrath_actor_view_44e6a0[];
extern unsigned char norrath_actor_view_44f4c8[];
extern unsigned char norrath_actor_view_485150[];
extern unsigned char norrath_actor_view_485410[];
extern unsigned char norrath_actor_view_485418[];
extern unsigned char norrath_actor_view_4816d8[];
extern unsigned char norrath_actor_view_480eec[];
extern unsigned char norrath_actor_view_480ef0[];
extern unsigned char norrath_actor_view_4813a4[];
extern unsigned char norrath_actor_view_4813a8[];
extern unsigned char norrath_actor_view_4813ac[];
extern unsigned char norrath_actor_view_48113c[];
extern unsigned char norrath_actor_view_481140[];
extern unsigned char norrath_actor_view_481a08[];
extern unsigned char norrath_actor_view_481a0c[];
extern unsigned char norrath_actor_view_481a10[];
extern unsigned char norrath_actor_view_4844a4[];
extern unsigned char norrath_actor_view_484f98[];
extern unsigned char norrath_actor_view_484318[];
extern unsigned char norrath_actor_view_4837a8[];
extern unsigned char norrath_actor_view_4837b0[];
extern unsigned char norrath_actor_view_4837b8[];
extern unsigned char norrath_actor_view_4837c0[];
extern unsigned char norrath_actor_view_4837c8[];
extern unsigned char norrath_actor_view_4837d0[];
extern unsigned char norrath_actor_view_4837d8[];
extern unsigned char norrath_actor_view_460710[];
extern unsigned char norrath_actor_view_460720[];
extern unsigned char norrath_actor_view_460730[];
extern unsigned char norrath_actor_view_460740[];
extern unsigned char norrath_actor_view_47fb54[];
extern unsigned char norrath_actor_view_486878[];
extern unsigned char norrath_actor_view_50bfd0[];
extern unsigned char norrath_actor_view_483ad0[];
}
