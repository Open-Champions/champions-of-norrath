// Observed: Init__t8VIVector1Z18VISceneDisplayElemi at 0x01224f40 is 140 bytes and returns an int.
// With (this, count): when the word at this+8 is nonzero it calls
// DestroyPool__t8VIVector1Z18VISceneDisplayElem(this); a negative result makes the function
// return -1 at once. Otherwise (including when the word was zero) it re-reads this+8, calls
// __builtin_vec_delete on it if still nonzero, then zeroes this+0, this+8 and this+4. It then
// stores count at this+4, calls Allocate__t8VIVector1Z18VISceneDisplayElem(this) and returns -1
// when that result is negative, else 0.
// Inferred: the word at this+8 is the element storage pointer and this+4 the requested
// capacity; this+0 is some used-length. Only the code shape is evidence for these roles.
// Unknown: the element layout and the value ranges of DestroyPool/Allocate results.
struct Container {
    int word0;
    int capacity;
    void* storage;
};

extern "C" {
int DestroyPool__t8VIVector1Z18VISceneDisplayElem(Container* self);
int Allocate__t8VIVector1Z18VISceneDisplayElem(Container* self);
void __builtin_vec_delete(void* pointer);

int Init__t8VIVector1Z18VISceneDisplayElemi(Container* self, int count) {
    if (self->storage != 0) {
        if (DestroyPool__t8VIVector1Z18VISceneDisplayElem(self) < 0) return -1;
        if (self->storage != 0) __builtin_vec_delete(self->storage);
    }
    self->word0 = 0;
    self->storage = 0;
    self->capacity = 0;
    self->capacity = count;
    return Allocate__t8VIVector1Z18VISceneDisplayElem(self) < 0 ? -1 : 0;
}
}
