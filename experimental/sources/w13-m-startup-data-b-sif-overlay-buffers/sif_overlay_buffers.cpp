// Observed data of the overlay SIF/IOP command block. The image holds these
// LOCAL objects at 0x011edb00 (sSend, 512 bytes), 0x011edd00 (sRecv, 512 bytes),
// 0x011edf00 (sFunctionIndex, an 8-byte slot), 0x011edf08 (sIopFunctionData, 512
// bytes) and 0x011ee108 (sSifClient, 56 bytes, up to the next symbol at 0x011ee140).
// The image bytes of all five spans are zero before any startup code runs.
// Inferred: sSend and sRecv are the 128-word argument and reply blocks of one
// SIF RPC call; sIopFunctionData is the argument array that SetIopFunction__FUii
// copies into sSend.
// Unknown: the second word of the 8-byte slot, and the fields of sSifClient. Its
// type is not established here, so it is held as bytes of the observed size.
// A second set of LOCAL names (sSend at 0x011e25c0, sRecv at 0x011e2600 and so
// on) is a different object and is not defined by this unit.
// The norrath_overlay_* spellings name the same addresses as the plain names here.
// They are source aliases of one storage object, not link-time symbol aliases.
// Nothing here ran on the EE.
extern "C" {
alignas(8) int sSend[128];
alignas(8) int sRecv[128];
alignas(8) int sFunctionIndex[2];
alignas(8) int sIopFunctionData[128];
alignas(8) unsigned char sSifClient[56];
#if !defined(__APPLE__)
extern int norrath_overlay_sSend_11edb00[128] __attribute__((alias("sSend")));
extern int norrath_overlay_sIopFunctionData_11edf08[128] __attribute__((alias("sIopFunctionData")));
#endif
}
