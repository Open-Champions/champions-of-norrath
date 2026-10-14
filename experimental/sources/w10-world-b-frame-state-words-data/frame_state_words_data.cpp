// Four zero-initialised 32-bit words from the private image's .data, accessed only as whole
// words (lw/sw), addresses from its symbol table:
//   framedone   0x004865d0   next to the keyboard state (lastkey, lasttime, inesc); the
//                            keyboard key-fetch code uses it
//   inesc       0x004865d4   used by the translated-key fetch code
//   lastCE      0x00486628   next to lastlastCE (0x0048662c); the real-time comm
//                            code near the comm routines reads and writes both
//   framestart  0x00486670   written once by the comm processing code
// The meanings are inferred from the neighbouring symbols only. Unknown: value ranges, and
// whether the first two are flags or counters.
extern "C" {
int framedone;
int inesc;
int lastCE;
int framestart;
}
