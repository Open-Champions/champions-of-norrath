// Observed initial values of single player-state objects. activePlayer
// (0x48453c, 4 bytes), isNetGame (0x484c70, 4), gamePlayersPresent (0x484f90, 4)
// and gameHidePlayers (0x484f28, 1) start at zero; so does wrathHammerCount
// (0x483f10, 1). playerSelectSuccessful (0x485ec8, 1) starts at 1.
// globalCharLightScale (0x483ab4, 4) holds the float 1.0. playerStartPos
// (0x429410) is 48 zero bytes. Section placement of the zero objects (.data
// versus .bss) is not reproduced here.
extern "C" {
int activePlayer = 0;
int isNetGame = 0;
int gamePlayersPresent = 0;
unsigned char gameHidePlayers = 0;
unsigned char wrathHammerCount = 0;
unsigned char playerSelectSuccessful = 1;
float globalCharLightScale = 1.0f;
alignas(4) unsigned char playerStartPos[48] = {};
}
