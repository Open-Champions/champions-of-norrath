#include "dtor.cpp"

static int order[8];
static int count;
static void* destroyed;
static void* vec_deleted;
static void* deleted;
static int clear_on_destroy;
static int storage_off = 4;

extern "C" void DestroyPool__t7VIArray1Z7VIPlane(void* object) {
    order[count++] = 1;
    destroyed = object;
    if (clear_on_destroy) *reinterpret_cast<void**>(static_cast<char*>(object) + storage_off) = nullptr;
}
extern "C" void __builtin_vec_delete(void* pointer) { order[count++] = 2; vec_deleted = pointer; }
extern "C" void __builtin_delete(void* pointer) { order[count++] = 3; deleted = pointer; }

static void reset() { count = 0; destroyed = vec_deleted = deleted = nullptr; clear_on_destroy = 0; }

int main() {
    alignas(8) char object[16] = {};
    char block[4];
    void** slot = reinterpret_cast<void**>(object + 4);
    // Empty storage: nothing is destroyed or deleted.
    reset();
    _$_t7VIArray1Z7VIPlane(object, 0);
    if (count != 0) return 1;
    reset();
    _$_t7VIArray1Z7VIPlane(object, 1);
    if (count != 1 || order[0] != 3 || deleted != object) return 2;
    // Storage present: destroy then vector-delete.
    reset(); *slot = block;
    _$_t7VIArray1Z7VIPlane(object, 0);
    if (count != 2 || order[0] != 1 || order[1] != 2 || destroyed != object || vec_deleted != block || deleted) return 3;
    reset(); *slot = block;
    _$_t7VIArray1Z7VIPlane(object, 3);
    if (count != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3 || deleted != object) return 4;
    reset(); *slot = block;
    _$_t7VIArray1Z7VIPlane(object, 2);
    if (count != 2 || deleted) return 5;
    // Destroy hook cleared the word: vector delete is skipped.
    reset(); *slot = block; clear_on_destroy = 1;
    _$_t7VIArray1Z7VIPlane(object, 0);
    if (count != 1 || order[0] != 1 || vec_deleted) return 6;
    return 0;
}
