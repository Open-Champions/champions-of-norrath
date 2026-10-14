// Host comparison only. Not an EE build input.
const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void slot_fn_0() asm("__tf9type_info");
extern "C" void slot_fn_0() { hit = "__tf9type_info"; }
extern "C" void slot_fn_1() asm("_$_9type_info");
extern "C" void slot_fn_1() { hit = "_$_9type_info"; }
struct VirtualSlot { unsigned adjustment; void (*function)(); };
extern "C" VirtualSlot norrath_rtti_entities_a_view_453850[3];
int main() {
    if (norrath_rtti_entities_a_view_453850[0].adjustment != 0x0u) return 1;
    hit = "";
    norrath_rtti_entities_a_view_453850[0].function();
    if (!same(hit, "__tf9type_info")) return 1;
    if (norrath_rtti_entities_a_view_453850[1].adjustment != 0x0u) return 1;
    hit = "";
    norrath_rtti_entities_a_view_453850[1].function();
    if (!same(hit, "_$_9type_info")) return 1;
    if (norrath_rtti_entities_a_view_453850[2].adjustment != 0x0u) return 1;
    if (norrath_rtti_entities_a_view_453850[2].function != nullptr) return 1;
    return 0;
}
