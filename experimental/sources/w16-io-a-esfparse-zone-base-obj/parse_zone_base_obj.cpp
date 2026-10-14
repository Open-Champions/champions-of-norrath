// Observed: ParseZoneBaseObj__10VIESFParse at 0x011750f8, 284 bytes. Argument: parse (a0: scene word at +24, current-zone word at
// +28, object file word at +36). It keeps a 16-byte VIArray1<int> on its stack and in order:
//   1. constructs it with __t7VIArray1Zi(array);
//   2. RemoveActorsFromZone__7VISceneR6VIZoneRt7VIArray1Zi(scene, zone, array);
//   3. ParseZoneTree__10VIESFParseR10VIZoneTree(parse, zone + 68);
//   4. ParseZonePreTranslations__10VIESFParseR6VIZone(parse, zone);
//   5. ParseZoneRooms__10VIESFParseR6VIZone(parse, zone);
//   6. ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(parse, zone + 100);
//   7. ParseZoneStaticTable__10VIESFParseRt7VIArray1Z21VIZoneStaticTableElem(parse, zone + 108);
//   8. when ObjectVersion__9VIObjFile(file) is not zero: ParseZoneFlora__10VIESFParse(parse), then
//      Precalc__17VIZoneRadialFloraR7VIScene(zone + 116, scene) (result ignored);
//   9. UpdateActors__7VISceneRt7VIArray1Zi(scene, array), then ReadEnd__9VIObjFile(file).
// A negative result from steps 2 to 7, the flora parse, UpdateActors or ReadEnd stops the sequence; in every case the array is
// destroyed with _$_t7VIArray1Zi(array, 2) and the result is -1 after a stop and 0 after a complete run. The zone and scene words
// are read again before each call that uses them.
// Inferred: this reads the base object of one zone: it clears the zone's current actors, then parses its tree, translations, rooms,
// resource and static tables and (from a later file version) flora, and re-adds the actors recorded in the array.
// Unknown: the member names, the element type behind the first array (Zi suggests int) and what version zero versus later means.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void __t7VIArray1Zi(void* array);
void _$_t7VIArray1Zi(void* array, int flags);
int RemoveActorsFromZone__7VISceneR6VIZoneRt7VIArray1Zi(void* scene, void* zone, void* array);
int UpdateActors__7VISceneRt7VIArray1Zi(void* scene, void* array);
int ParseZoneTree__10VIESFParseR10VIZoneTree(void* parse, void* tree);
int ParseZonePreTranslations__10VIESFParseR6VIZone(void* parse, void* zone);
int ParseZoneRooms__10VIESFParseR6VIZone(void* parse, void* zone);
int ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(void* parse, void* table);
int ParseZoneStaticTable__10VIESFParseRt7VIArray1Z21VIZoneStaticTableElem(void* parse, void* table);
int ParseZoneFlora__10VIESFParse(void* parse);
void Precalc__17VIZoneRadialFloraR7VIScene(void* flora, void* scene);
unsigned ObjectVersion__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);

int ParseZoneBaseObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    alignas(8) unsigned char array[16];
    __t7VIArray1Zi(array);
    int result = -1;
    if (RemoveActorsFromZone__7VISceneR6VIZoneRt7VIArray1Zi(NORRATH_WORD_ADDRESS(words[6]), NORRATH_WORD_ADDRESS(words[7]), array) >= 0 &&
        ParseZoneTree__10VIESFParseR10VIZoneTree(parse, static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7])) + 68) >= 0 &&
        ParseZonePreTranslations__10VIESFParseR6VIZone(parse, NORRATH_WORD_ADDRESS(words[7])) >= 0 &&
        ParseZoneRooms__10VIESFParseR6VIZone(parse, NORRATH_WORD_ADDRESS(words[7])) >= 0 &&
        ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(parse, static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7])) + 100) >= 0 &&
        ParseZoneStaticTable__10VIESFParseRt7VIArray1Z21VIZoneStaticTableElem(parse, static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7])) + 108) >= 0) {
        bool proceed = true;
        if (ObjectVersion__9VIObjFile(NORRATH_WORD_ADDRESS(words[9])) != 0) {
            if (ParseZoneFlora__10VIESFParse(parse) < 0) {
                proceed = false;
            } else {
                Precalc__17VIZoneRadialFloraR7VIScene(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7])) + 116, NORRATH_WORD_ADDRESS(words[6]));
            }
        }
        if (proceed && UpdateActors__7VISceneRt7VIArray1Zi(NORRATH_WORD_ADDRESS(words[6]), array) >= 0 &&
            ReadEnd__9VIObjFile(NORRATH_WORD_ADDRESS(words[9])) >= 0) {
            result = 0;
        }
    }
    _$_t7VIArray1Zi(array, 2);
    return result;
}
}
