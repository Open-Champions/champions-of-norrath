// Observed IsT10K at 0x002d1068, 64 bytes, a local routine of the system-configuration (Scf) library.
// It looks at the first byte of the 15-byte object gRomName (0x00444858). While that byte is zero it calls
// GetRomName (0x002d0ff8), which fills the object from a file once. The result is 1 when the byte at
// offset 4 is the ASCII letter 'T', otherwise 0. The return value of GetRomName is not used.
// Inferred: gRomName holds the ROM version string read from the console, and a 'T' in that position
// marks a test or tool machine; callers use the result to choose between gConfig and the OSD settings.
// Unknown: the exact text layout of the ROM string and the meaning of the name T10K beyond that test.
// Callers: sceScfGetTimeZone and sceScfGetSummerTime. GetRomName and gRomName are not authored here.
extern "C" {
extern char gRomName[15];
void* GetRomName();

int IsT10K() {
    if (gRomName[0] == 0) GetRomName();
    return gRomName[4] == 'T';
}
}
