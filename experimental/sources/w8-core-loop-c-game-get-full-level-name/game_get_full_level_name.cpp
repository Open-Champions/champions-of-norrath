// gameGetFullLevelName at 0x001d1108, 76 bytes, in SLUS_205.65.
// Observed: works on the byte array levelName (0x0034b660, 322 bytes) with two text fields read
// at byte offsets 81 and 161. When the byte at offset 161 is nonzero the destination receives
// sprintf(out, "%s_%s", levelName + 81, levelName + 161), a tail call returning sprintf's
// result. Otherwise it is strcpy(out, levelName + 81), whose result is not returned: the
// function returns without setting a value on that path. The format text is the five
// characters %s_%s, read from the image.
// Inferred: the return type is void-like for callers on the strcpy path. Unknown: what the
// fields at 81 and 161 hold, and how the fields before 81 are used.
extern "C" char levelName[];
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" char* strcpy(char* destination, const char* source);

extern "C" void gameGetFullLevelName__FPc(char* out) {
    if (levelName[161] != 0) {
        sprintf(out, "%s_%s", levelName + 81, levelName + 161);
        return;
    }
    strcpy(out, levelName + 81);
}
