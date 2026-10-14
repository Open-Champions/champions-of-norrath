// Observed scheduler counters, both local .sdata objects, 4 file bytes, zero.
// GP 0x004879f0.
//   nmTasks     0x00484478  displacement -13688
//   currentTask 0x0048447c  displacement -13684
// Original binding is local. These definitions are global so the other
// scheduler units can resolve them. The reviewed -G0 profile does not
// recreate .sdata. Both values are zero.
extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4];
}
