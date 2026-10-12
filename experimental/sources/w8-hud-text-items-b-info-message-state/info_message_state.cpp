// Observed storage for the on-screen info message.
// infoMessage is at 0x007d0798 in the uninitialized region. The next symbol,
// iconAlpha, starts 200 bytes later, so 100 16-bit characters is the upper
// bound that the symbol layout allows; the real capacity is not established.
// infoMessageTimer is a halfword at 0x00484b18 and infoMessageAlways is a byte at
// 0x00484b1a. Both hold zero in the image. hudSetInfoMessage__FPUsiic writes
// all three, hudClearInfoMessage__Fv clears the timer.
extern "C" {
unsigned short infoMessage[100];
unsigned short infoMessageTimer = 0;
signed char infoMessageAlways = 0;
}
