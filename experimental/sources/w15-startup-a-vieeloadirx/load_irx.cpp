// Observed: VIEELoadIrx__FPCciT0 at 0x010f9be8 is 116 bytes and returns an int (the last
// sceSifLoadModule result). Arguments (name, second, third); the mangled name makes the
// third the same type as the first, a C string pointer. It prints "Loading module %s\n" with
// the name, formats "cdrom0:\MODULES\%s;1" with the name into a 256-byte stack buffer via
// VIString::snprintf (the buffer size 256 is passed), and calls
// sceSifLoadModule(buffer, second, third). While that result is negative it calls
// sceSifLoadModule again with the same arguments, without reformatting or printing again;
// the first non-negative result is returned.
// Inferred: second is the length of the module argument string and third the argument string
// itself, as in the SDK prototype; the module names live in a MODULES directory on the disc.
// Unknown: the callers' exact argument conventions and whether the endless retry can end
// (it depends on the IOP side).
extern "C" {
int printf(const char* format, ...);
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...);
int sceSifLoadModule(const char* path, int argument_length, const char* arguments);

int VIEELoadIrx__FPCciT0(const char* name, int length, const char* arguments) {
    char path[256];
    printf("Loading module %s\n", name);
    snprintf__8VIStringPciPCce(path, 256, "cdrom0:\\MODULES\\%s;1", name);
    int result;
    do {
        result = sceSifLoadModule(path, length, arguments);
    } while (result < 0);
    return result;
}
}
