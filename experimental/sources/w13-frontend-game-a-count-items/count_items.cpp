// Observed countItems__FP10GameObject at 0x001b48b0, 276 bytes (a file-local function in the image).
// Argument: a game object. Its dynamic type is read by calling the function word at (word at object
// + 72) + 4 with the object itself (a null object calls __throw_bad_typeid instead), and compared
// with __tf8ItemProp() through __eq__C9type_infoRC9type_info; any other type returns at once.
// Then an object whose 64-bit word at +336 has bit 0x8000000000 set also returns at once.
// Otherwise fullCount is incremented, and a reference object is chosen: the world object word at
// byte 21872 of player record 0 (player + 21872) or, when that is null, that of record 1
// (player + 45856); both null returns. With the three floats at +16 of the reference and of this
// object, the distance d = sqrt(dx*dx + dy*dy + dz*dz) (x, y, z in that order, reference minus this)
// replaces maxDistance when maxDistance < d, and the object is then stored in itemToDestroy.
// Inferred: a caller walks the objects and ends with the item farthest from the first player; the
// names fullCount, itemToDestroy and maxDistance are the image's own local symbols.
// Unknown: the meaning of flag bit 39, the initial value of maxDistance (a .data float).
extern "C" {
extern float maxDistance;
extern int fullCount;
extern void* itemToDestroy;
extern unsigned char player[];
void __throw_bad_typeid();
void* __tf8ItemProp();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
}

#ifdef W13_FRONT_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
// 32-bit pointer words of the image become native pointers through this on the host check.
inline __UINTPTR_TYPE__ widen(unsigned word) {
#ifdef W13_FRONT_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
typedef void* (*TypeFunction)(void* object);
}

extern "C" void countItems__FP10GameObject(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    void* type = nullptr;
    if (object == nullptr) {
        __throw_bad_typeid();
    } else {
        const unsigned char* table = reinterpret_cast<const unsigned char*>(widen(load<unsigned>(object, 72)));
        type = reinterpret_cast<TypeFunction>(widen(load<unsigned>(table, 4)))(object);
    }
    if (!__eq__C9type_infoRC9type_info(type, __tf8ItemProp())) return;
    if ((load<unsigned long long>(object, 336) & 0x8000000000ull) != 0) return;
    fullCount = fullCount + 1;
    unsigned char* reference = reinterpret_cast<unsigned char*>(widen(load<unsigned>(player, 21872)));
    if (reference == nullptr) {
        reference = reinterpret_cast<unsigned char*>(widen(load<unsigned>(player, 45856)));
        if (reference == nullptr) return;
    }
    const float dx = load<float>(reference, 16) - load<float>(object, 16);
    const float dy = load<float>(reference, 20) - load<float>(object, 20);
    const float dz = load<float>(reference, 24) - load<float>(object, 24);
    const float distance = square_root(dx * dx + dy * dy + dz * dz);
    if (maxDistance < distance) {
        maxDistance = distance;
        itemToDestroy = self;
    }
}
