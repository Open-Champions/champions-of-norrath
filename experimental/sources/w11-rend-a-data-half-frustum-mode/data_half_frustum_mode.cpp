// Observed halfFrustumMode at 0x00484134, one byte in .data, file byte zero (next object
// waterPatch at 0x00484138). The authored frustum transform reads it as an unsigned byte. The
// image places it in .data; this source gives it the reviewed zero-initialised storage, so the
// placement is not reproduced. What a nonzero value selects is described at its reader, not
// here. Nothing here was executed on the EE.
extern "C" {
unsigned char halfFrustumMode;
}
