// The three words objectSaveAddToStringPool and objectSaveGetFromStringPool use,
// all in .bss, file image zero:
//   poolStart  0x48689c  start of the save string pool (zero until one is made)
//   poolEnd    0x4868a0  end of the used part
//   poolOffset 0x4868a4  base added to an index by objectSaveGetFromStringPool
// Observed: the add routine reads and writes only poolStart and poolEnd, the get
// routine reads only poolOffset. Unknown: who sets poolOffset (objectClear zeroes
// pool words; the loader is not in this package).
extern "C" {
char* poolStart;
char* poolEnd;
char* poolOffset;
}
