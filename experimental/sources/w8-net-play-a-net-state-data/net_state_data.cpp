// Observed network and player-count state words.
//   validLocalPlayer (0x484f40), nmPlayersAlive (0x484f44),
//   localNmPlayersAlive (0x484f48), lastNmLocalPlayersAlive (0x484f4c): four
//   bytes each, all start at zero.
//   playersAlive (0x42a3e8) and localPlayersAlive (0x42a3d8): 16 bytes each,
//   zero at start.
//   comm_handle (0x486618) starts at -1; commRVInternal (0x486608) at 25;
//   commRVThresh (0x48660c) at 10000; commPV (0x486610) at 100.
//   norrath_player_select_level_bytes_484c40 is the 8 bytes at 0x484c40: the
//   text "kelsel2" and its terminator, read by launchPlayerSelect.
// The names of the first group follow the original symbols. The last name is a
// view name for an address with no symbol of its own.
extern "C" {
int validLocalPlayer = 0;
int nmPlayersAlive = 0;
int localNmPlayersAlive = 0;
int lastNmLocalPlayersAlive = 0;
int playersAlive[4] = {};
int localPlayersAlive[4] = {};
int comm_handle = -1;
int commRVInternal = 25;
int commRVThresh = 10000;
int commPV = 100;
unsigned char norrath_player_select_level_bytes_484c40[8] = {'k', 'e', 'l', 's', 'e', 'l', '2', 0};
}
