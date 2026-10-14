// Observed in the pinned SLUS-20565 image. Both objects are local symbols in
// the 0x011exxxx range; sizes are the span to the next symbol and the file
// image holds zero for every byte.
//
//   0x011e2dd0  1024  sGameOptions   returned by the game-options getter
//   0x011e7d60     8  sCdRmode       the display code stores 0, 1, 0, 0 into the
//                                    first four bytes before each read
//
// Inferred: none. Unknown: how much of sGameOptions the program uses and the
// reason sCdRmode spans eight bytes when only four are written.
extern "C" {
alignas(16) char sGameOptions[1024];
alignas(8) unsigned char sCdRmode[8];
}
