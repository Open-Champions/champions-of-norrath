/* Floor storage already imported by the authored floor, lighting, and
 * sprite routines. Original names and addresses are from the pinned
 * SLUS-20565 symbol table. A zero st_size is the span to the next symbol
 * in that section. Image bytes for every object here are zero, except
 * floorDrawEnable, whose .sdata byte is 1.
 *
 * The reviewed -G0 profile does not recreate .sdata or .sbss. Zero words
 * are ordinary zero storage. This file does not define the VU program
 * between vu1floorstart and vu1floorend.
 */
extern "C" {
/* g_BloodLevel, .sdata 0x00484498, st_size 4, bytes 0. */
alignas(4) unsigned char norrath_floor_blood_level_word_view[4];
/* spriteCount, .sdata 0x004844d8, st_size 4, bytes 0. */
alignas(4) unsigned char norrath_floor_sprite_count_word_view[4];
/* fadePacket, .sdata 0x004844dc, st_size 4, bytes 0. */
alignas(4) unsigned char norrath_floor_fade_packet_word_view[4];
/* freeList, .sbss 0x00486a68, span 4 to freeNodes. */
alignas(4) unsigned char norrath_floor_free_list_word_view[4];
/* interfaceMode, .sdata 0x00483aac, st_size 4, bytes 0. */
alignas(4) unsigned char norrath_floor_light_interface_mode_word[4];
/* scene, .bss 0x004f4060, span 56 to savedScene. lightInit writes through byte 55. */
alignas(8) unsigned char norrath_floor_light_scene_view[56];
/* expandList, .bss 0x007940b0, span 320 to sprPacket. 16 records of 20 bytes. */
alignas(16) unsigned char norrath_floor_expand_list_view[320];
/* sprGrid, .bss 0x007ac1f0, span 3600 to nodes. floorClear clears 3600 bytes. */
alignas(16) unsigned char norrath_floor_sprite_grid_view[3600];
/* sprPacket, .bss 0x007941f0, span 98304 to sprGrid. 128 records of 768 bytes. */
alignas(16) unsigned char norrath_floor_sprite_packet_view[98304];
/* lightGrid, .bss 0x004f40f0, span 32400 to saveBestList. 90 by 90 words. */
alignas(16) unsigned char norrath_floor_light_grid_view[32400];
/* floorDrawEnable, .sdata 0x004844e0, st_size 1, byte 1. */
alignas(1) unsigned char floorDrawEnable = 1;
/* tex.678, .bss 0x007940a0, span 16 to expandList. Compiler temporary; the
 * authored name is inferred. ResourceMap's constructor writes 12 bytes. */
alignas(16) unsigned char norrath_floor_sprite_texture_map[16];
/* _$tmp_0.679, .sdata 0x004844e4, st_size 4, bytes 0. Inferred name. */
alignas(4) unsigned char norrath_floor_sprite_texture_ready_word[4];
/* Unnamed .rodata at 0x00464430. Not the startup engine.lmp at 0x0047b0f0. */
char norrath_floor_sprite_texture_group[] = "engine.lmp";
/* Unnamed .rodata at 0x00464440. */
char norrath_floor_sprite_texture_name[] = "floorsprite.tex";
}
