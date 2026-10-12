// Observed in the pinned SLUS-20565 image. Addresses, sizes and initial
// values were read from the symbol table and the file image; sizes of the
// bss words are the access width, which agrees with the gap to the next
// symbol. Every object starts as zero. Section placement follows the build
// profile (-G0), not the original small-data split.
//
//   0x00486d1c  4  requestToExit            word, loaded once, stored zero four times
//   0x004869a0  4  restrictChars            word
//   0x00486b18  4  saveOk                    word
//   0x00486a9c  4  startFrame                word
//   0x00486b68  4  textBodyLen               word
//   0x00486dac  4  waiting                   word
//   0x00486a98  4  waitingForStreamSpool     word, only ever stored
//   0x00486d2e  1  startExitFade             byte
//   0x00485088  4  scriptFrame               word
//   0x00484050  4  startInhibit              word
//   0x00484cb4  2  startScorePos             signed halfword (also read unsigned)
//   0x004840a0  1  texturesAreLoading        signed byte
//   0x004840a1  1  texturesAreDecompressing  signed byte
//
// Inferred: none. Unknown: what each value means and which code is meant to
// be the sole writer. startScorePos is shared with the music code, which
// declares it as a short.
extern "C" {
int requestToExit;
int restrictChars;
int saveOk;
int startFrame;
int textBodyLen;
int waiting;
int waitingForStreamSpool;
char startExitFade;
int scriptFrame;
int startInhibit;
short startScorePos;
signed char texturesAreLoading;
signed char texturesAreDecompressing;
}
