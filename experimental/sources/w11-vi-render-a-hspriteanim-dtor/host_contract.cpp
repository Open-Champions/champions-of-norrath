#include "dtor.cpp"

static void* cleared;
static void* deleted;
static int order, clear_order, delete_order;
extern "C" int Clear__13VIHSpriteAnim(void* o) { cleared = o; clear_order = ++order; return 0; }
extern "C" void __builtin_delete(void* p) { deleted = p; delete_order = ++order; }

int main() {
    int object;
    for (int flags = 0; flags < 8; ++flags) {
        cleared = deleted = 0; order = clear_order = delete_order = 0;
        _$_13VIHSpriteAnim(&object, flags);
        if (cleared != &object || clear_order != 1) return 1 + flags;
        if ((flags & 1) != 0) {
            if (deleted != &object || delete_order != 2) return 10 + flags;
        } else if (deleted != 0) {
            return 20 + flags;
        }
    }
    return 0;
}
