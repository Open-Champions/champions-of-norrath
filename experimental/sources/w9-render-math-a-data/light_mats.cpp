// Observed lightMats at 0x008d5b90. targetObjs begins 0x1b0 bytes later. The authored
// select routine writes ten rows of nine singles (360 bytes) into the front of it. The
// remaining 72 bytes are not explained by any authored caller. The 432-byte extent is the
// symbol gap.
extern "C" {
unsigned char lightMats[432];
}
