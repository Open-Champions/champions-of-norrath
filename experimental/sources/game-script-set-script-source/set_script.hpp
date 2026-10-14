#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(unsigned long long) == 8 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "Norrath word, address and native status representations");
extern "C" {
// The existing scriptStart word view; no storage or original binding is defined.
extern unsigned char norrath_script_current_amx_word_view[];
// Exact existing genuine provider interfaces. Their statuses are ignored here.
unsigned long long norrath_amx_initialize(void* amx, void* program);
unsigned long long norrath_amx_register_game_natives(void* amx);
void norrath_script_set_script(unsigned char* script);
}
