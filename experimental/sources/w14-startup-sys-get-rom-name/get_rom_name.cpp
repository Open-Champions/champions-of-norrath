// Observed GetRomName at 0x002d0ff8, 112 bytes, a local routine of the system-configuration (Scf) library.
// It fills the 15-byte object gRomName (0x00444858) from a file, once: nothing happens when the first byte is
// already non-zero. Otherwise it opens the file named by the string "rom0:ROMVER" (at 0x0047b148) for reading
// (open flag 1), and when the descriptor is not negative reads 14 bytes into gRomName and closes the descriptor.
// The routine returns the address of gRomName in every case, including when the open failed or the read came
// back short; the return value of sceRead is not examined.
// Inferred: the 14 bytes are the ROM version string of the console; the file belongs to the ROM device.
// Unknown: the layout of the string and whether sceRead can be short in practice.
// Callers: IsT10K. sceOpen, sceRead and sceClose are the libc-style file calls of the image, not authored here.
extern "C" {
extern char gRomName[15];
int sceOpen(const char* path, int flags);
int sceRead(int fd, void* buf, int size);
int sceClose(int fd);

char* GetRomName() {
    if (gRomName[0] == 0) {
        int fd = sceOpen("rom0:ROMVER", 1);
        if (fd >= 0) {
            sceRead(fd, gRomName, 14);
            sceClose(fd);
        }
    }
    return gRomName;
}
}
