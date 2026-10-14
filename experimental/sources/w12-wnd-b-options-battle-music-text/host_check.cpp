extern "C" int battle_music_text __asm__("VIWndOptions$sBattleMusicText");
int main() { return battle_music_text == 0 ? 0 : 1; }
