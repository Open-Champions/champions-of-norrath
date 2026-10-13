// Observed: VIWndOptions$sBattleMusicText is the four-byte word at
// 0x011e8bd0. Its initial image value is zero, and Init__12VIWndOptions
// stores zero to it. The symbol spelling suggests text, but its use/type
// beyond this observed word is unknown.
extern "C" {
int battle_music_text __asm__("VIWndOptions$sBattleMusicText") = 0;
}
