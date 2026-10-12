// Observed: allActs is five pointers, to act1, act2, act3, act4 and act5.
// MC_Init indexes it by act and reads 8-byte level records through the pointer.
// Inferred: none.
// Unknown: the record fields beyond the name word and the size word MC_Init uses.
extern "C" char act1[];
extern "C" char act2[];
extern "C" char act3[];
extern "C" char act4[];
extern "C" char act5[];
extern "C" char* allActs[5] = {act1, act2, act3, act4, act5};
