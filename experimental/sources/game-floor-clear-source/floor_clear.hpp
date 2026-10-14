#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "little endian views");
extern "C" {
// References only: this source neither allocates nor binds any of these views.
extern unsigned char norrath_floor_sprite_packet_view[];
extern unsigned char norrath_floor_sprite_grid_view[];
extern unsigned char norrath_floor_free_list_word_view[];
extern unsigned char norrath_floor_sprite_count_word_view[];
extern unsigned char norrath_floor_fade_packet_word_view[];
extern unsigned char norrath_floor_program_start_view[];
extern unsigned char norrath_floor_program_end_view[];
void* norrath_block_memory_set(void* destination, int value, unsigned byte_count);
void norrath_floor_clear();
}
