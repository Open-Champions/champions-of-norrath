// Observed lightInfo at 0x008d5e40, zero-size in .bss; the extent to the next symbol (startPos
// at 0x008d5fc0) is 0x180 (384) bytes. The authored initializer treats it as 96 floats.
// Unknown: the internal record layout.
extern "C" {
float lightInfo[96];
}
