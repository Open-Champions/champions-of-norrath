// Observed small zero-initialised words in the pinned SLUS-20565 image.
// All six are in the initialised data area and hold zero in the file.
//   gameControlDontResetPause 0x00483f20, word; five stores and one load.
//   cheatOn                   0x00483f54, byte; read once as signed.
//   helpMessageOn             0x00483f55, byte; read as signed, 7 loads.
//   language                  0x004844b4, byte; read as signed (71 loads)
//                             and unsigned (5), stored twice.
//   controlPaused             0x00484b04, word; three stores, four loads.
//   hudNoController           0x00484b73, byte; two stores, one load.
// Inferred: the element widths, from the load/store opcodes. Signedness of
// cheatOn, helpMessageOn and language follows the load opcodes; the
// stored values may be flags or small codes, which this unit does not decide.
// Unknown: what values `language` takes and whether controlPaused is a
// countdown or a flag. Under the build profile (-G0) these land in .bss/.data
// by the compiler's rules, not in the original .sdata placement.
extern "C" {
int gameControlDontResetPause = 0;
signed char cheatOn = 0;
signed char helpMessageOn = 0;
signed char language = 0;
int controlPaused = 0;
unsigned char hudNoController = 0;
}
