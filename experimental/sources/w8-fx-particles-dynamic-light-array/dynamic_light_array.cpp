// DynamicLightArray at 0x006e9d60, 1088 BSS bytes (the gap to routeQueue), zero in
// the file image. worldInitWorld__FP12_worldHeader walks it as 16 records of 68
// bytes: the words at +0 and +4 of each record are cleared and the light record
// at +8 is passed to lightDeactivate__FP5Light. Sixteen times 68 is 1088. The
// remaining fields are not established here. The reviewed -G0 profile places it in
// BSS.
extern "C" {
unsigned char DynamicLightArray[16 * 68];
}
static_assert(sizeof(DynamicLightArray) == 1088u, "DynamicLightArray extent");
