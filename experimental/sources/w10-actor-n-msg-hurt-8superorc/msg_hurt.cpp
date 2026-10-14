// Observed msg_hurt__8SuperOrcfP10GameObjecti at 0x0022ba60, 112 bytes. Calls the Creature
// version, msg_hurt__8CreaturefP10GameObjecti, with the same arguments. Then, when the text in
// engineCurrentLevelName differs from "gothic1" (strcmp non-zero), copies the 9-byte text
// "SuperOrc" (with terminator) into bossMeterName and stores the float at object+560 converted
// to an integer into bossMeterCurrent. When the level name is "gothic1" nothing more happens.
// The Creature call's return value is not used; this function returns nothing.
// Unknown: why that one level is excluded. Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern char engineCurrentLevelName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int kind);
int strcmp(const char* a, const char* b);
}

typedef float Single __attribute__((may_alias));

extern "C" void msg_hurt__8SuperOrcfP10GameObjecti(void* self, float amount, void* source, int kind) {
    unsigned char* object = static_cast<unsigned char*>(self);
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, kind);
    if (strcmp(engineCurrentLevelName, "gothic1") != 0) {
        __builtin_memcpy(bossMeterName, "SuperOrc", 9);
        bossMeterCurrent = static_cast<int>(*reinterpret_cast<Single*>(object + 560));
    }
}
