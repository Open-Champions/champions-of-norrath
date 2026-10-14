// Observed: three 4-byte words in small data.
//   noAdvanceTime   0x00483868  zero in the file
//   resourceFrame   0x00483f88  file value 1
//   routeClearStart 0x00484190  zero in the file
// The network start/step/init routines read noAdvanceTime; the resource map and lump
// clearing routines use resourceFrame; the world clear routine uses routeClearStart.
// Inferred: none. Unknown: the meaning of resourceFrame's initial 1 (other authored views of
// the same address, such as the constructor packages' 0x483f88 views, are separate symbols).
extern "C" {
int noAdvanceTime = 0;
int resourceFrame = 1;
int routeClearStart = 0;
}
static_assert(sizeof(noAdvanceTime) == 4 && sizeof(resourceFrame) == 4 && sizeof(routeClearStart) == 4, "observed sizes");
