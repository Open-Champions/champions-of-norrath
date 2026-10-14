// Observed: ParseWorldZones__10VIESFParseR7VIWorld at 0x01176180 is 776 bytes. Arguments: parse (a0;
// current-zone word at +28, scene word at +24, object file word at +36) and world (a1, a world
// whose zone table sits at +72: a signed zone count in the first word and a pointer to the zone
// records in the second, records are 592 bytes). For each zone index below the count (the count
// word is read again after every zone) it:
//   1. stores the zone record address in the parse word at +28;
//   2. calls ReadBegin__9VIObjFileUlRUsT2(file, key, &tag, &second) where key is the 8-byte value
//      at zone+16 (loaded as a doubleword); a negative result or a tag other than 0x3200 returns -1,
//      otherwise ParseZoneBaseObj__10VIESFParse(parse) (negative returns -1);
//   3. calls the same ReadBegin with the 8-byte value at (the +28 word)+8; negative, or a tag other
//      than 0x3000, returns -1;
//   4. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x3100, returns -1; then for each
//      index below NumSubObjects__9VIObjFile(file) (called before every test) calls
//      ParseZoneResource__10VIESFParse(parse) (negative returns -1); then ReadEnd__9VIObjFile
//      (negative returns -1);
//   5. the same shape with tag 0x3290: for each sub-object a ReadBegin__9VIObjFileRUsT1 must give
//      tag 0x3270 and ParseZoneRoomActorsObj__10VIESFParse(parse) must not be negative, then
//      ReadEnd;
//   6. the same with tag 0x32b0: for each sub-object a ReadBegin must give tag 0x3290 and
//      ParseZoneRoomStaticLightingsObj__10VIESFParse(parse) must not be negative, then ReadEnd;
//   7. calls ReplaceProxyRooms__7VISceneR6VIZone(scene, zone) and
//      ReplaceStaticProxyActors__7VISceneR6VIZone(scene, zone) (each negative returns -1) and
//      ReadEnd (negative returns -1), then clears the parse word at +28.
// A negative result anywhere leaves the +28 word set and returns -1; finishing every zone (or a count
// of zero or less) returns 0.
// Inferred: tags 0x3200/0x3000 open a zone block (base object then a nested block), 0x3100 holds
// resources, 0x3290 the room actors and 0x32b0 the room static lightings; the keys read from the zone
// look like 64-bit identifiers (the EE long is 8 bytes, hence the Ul name). Unknown: the field
// names of the zone record, what the keys are, and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileUlRUsT2(void* file, unsigned long long key, unsigned short* tag, unsigned short* second);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ParseZoneBaseObj__10VIESFParse(void* parse);
int ParseZoneResource__10VIESFParse(void* parse);
int ParseZoneRoomActorsObj__10VIESFParse(void* parse);
int ParseZoneRoomStaticLightingsObj__10VIESFParse(void* parse);
int ReplaceProxyRooms__7VISceneR6VIZone(void* scene, void* zone);
int ReplaceStaticProxyActors__7VISceneR6VIZone(void* scene, void* zone);

int ParseWorldZones__10VIESFParseR7VIWorld(unsigned char* parse, unsigned char* world) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned* table = reinterpret_cast<unsigned*>(world + 72);
    unsigned short tag;
    unsigned short second;
    for (int i = 0; i < static_cast<int>(table[0]); ++i) {
        words[7] = table[1] + 592 * i;
        if (ReadBegin__9VIObjFileUlRUsT2(file, *reinterpret_cast<unsigned long long*>(NORRATH_WORD_ADDRESS(words[7] + 16)), &tag, &second) < 0) return -1;
        if (tag != 0x3200) return -1;
        if (ParseZoneBaseObj__10VIESFParse(parse) < 0) return -1;
        if (ReadBegin__9VIObjFileUlRUsT2(file, *reinterpret_cast<unsigned long long*>(NORRATH_WORD_ADDRESS(words[7] + 8)), &tag, &second) < 0) return -1;
        if (tag != 0x3000) return -1;
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (tag != 0x3100) return -1;
        for (int j = 0; j < NumSubObjects__9VIObjFile(file); ++j) {
            if (ParseZoneResource__10VIESFParse(parse) < 0) return -1;
        }
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (tag != 0x3290) return -1;
        for (int j = 0; j < NumSubObjects__9VIObjFile(file); ++j) {
            if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
            if (tag != 0x3270) return -1;
            if (ParseZoneRoomActorsObj__10VIESFParse(parse) < 0) return -1;
        }
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (tag != 0x32b0) return -1;
        for (int j = 0; j < NumSubObjects__9VIObjFile(file); ++j) {
            if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
            if (tag != 0x3290) return -1;
            if (ParseZoneRoomStaticLightingsObj__10VIESFParse(parse) < 0) return -1;
        }
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
        if (ReplaceProxyRooms__7VISceneR6VIZone(NORRATH_WORD_ADDRESS(words[6]), NORRATH_WORD_ADDRESS(table[1] + 592 * i)) < 0) return -1;
        if (ReplaceStaticProxyActors__7VISceneR6VIZone(NORRATH_WORD_ADDRESS(words[6]), NORRATH_WORD_ADDRESS(table[1] + 592 * i)) < 0) return -1;
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
        words[7] = 0;
    }
    return 0;
}
}
