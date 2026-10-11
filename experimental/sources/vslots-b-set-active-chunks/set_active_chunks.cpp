#include "../vslots-b-bytes/bytes.hpp"
// Observed setActiveChunks__8Creature at 0x113828, 104 bytes.
// Virtual slot byte 128 is called once. The creature vtable names that slot
// getModel__8Creature. A null return stops. A non-null return calls that same
// slot again, then modelGetActiveChunks with the second return and the word
// at object+244. The eight-byte return register is stored at object+552.
// Unknown: which half of that return the callee actually sets.
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
extern "C" void setActiveChunks__8Creature(void* self) {
    using namespace vslots_b;
    void* adjusted = nullptr;
    using ModelCall = void* (*)(void*);
    ModelCall model_of = slot_function<ModelCall>(self, 128u, &adjusted);
    if (model_of(adjusted) == nullptr) return;
    void* model = model_of(adjusted);
    const unsigned long long chunks =
        modelGetActiveChunks__FP12_modelHeaderi(model, signed_word(read_word(address(self) + 244u)));
    write_u64(address(self) + 552u, chunks);
}
