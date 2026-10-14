// Storage for five placeholder-named imports that older sources use for objects of the image. Each
// is defined here with the size, initial value and section kind observed in SLUS_205.65:
//   norrath_got_player_start_this_level_bytes   1 byte,  0x00484ca8 (original symbol gotPlayerStartThisLevel)
//   norrath_keep_player_positions_bytes         1 byte,  0x00486cc6 (original symbol keepPlayerPositions)
//   norrath_restore_broadcast_mask_word         4 bytes, 0x00483ac0 (original symbol objectIgnoreFlags), zero
//   norrath_menu_local_t_004844e8               float 1.0, 0x004844e8 (bit pattern 0x3f800000)
//   norrath_menu_word_00484514                  int 116,   0x00484514
// Both byte objects start at zero. The first three have an authored original-named definition in
// another unit; the placeholder names are separate objects in this relocatable build, so the two names
// do not alias. Whoever integrates must decide how the two names become one object before an
// executable is linked; this unit only supplies the missing definitions.
// Unknown: what the menu float and the menu word mean (the menu code uses them as a tween value and as
// a record stride respectively; this unit does not establish either).
extern "C" {
unsigned char norrath_got_player_start_this_level_bytes[1];
unsigned char norrath_keep_player_positions_bytes[1];
unsigned char norrath_restore_broadcast_mask_word[4];
float norrath_menu_local_t_004844e8 = 1.0f;
int norrath_menu_word_00484514 = 116;
}
