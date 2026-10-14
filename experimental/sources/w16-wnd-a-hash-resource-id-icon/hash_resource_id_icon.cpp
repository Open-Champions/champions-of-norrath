// Observed: VIHashResourceID_Icon__FPCc at 0x01053970 is 164 bytes. It copies a 38-byte format string from the
// image's constant data (the path of an icon texture file under the studio's asset directory, with one %s) to the
// stack, calls snprintf__8VIStringPciPCce(buffer, 512, format, name) with a 512-byte buffer, and returns
// VIHashResourceID(buffer).
// Inferred: builds the icon's full resource name from the short name and hashes it, as the icon lookups do.
// Unknown: the owner of the format string (kept here as a literal).
extern "C" {
unsigned VIHashResourceID(const char* name);
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...);

unsigned VIHashResourceID_Icon__FPCc(const char* name) {
    char buffer[512];
    const char format[] = "P:\\studio\\eqps2\\Game_Assets\\UI\\%s.tga";
    snprintf__8VIStringPciPCce(buffer, 512, format, name);
    return VIHashResourceID(buffer);
}
}
