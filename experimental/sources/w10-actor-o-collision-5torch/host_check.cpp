// Host contract for msg_collision__5TorchP10GameObject; not an EE input. Includes the source.
#include "msg_collision.cpp"
#include <cstring>
#include <initializer_list>

int frameNm;
static int calls;
static void *seen_target, *seen_source;
static float seen_a, seen_b;
static int seen_c, seen_d, seen_e, seen_f;
static bool seen_flag;
static unsigned char desc_bytes[72];
static void* seen_desc;
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float a, float b, int c, int d, int e, int f, void* description, bool flag) {
    ++calls; seen_target = target; seen_source = source; seen_a = a; seen_b = b;
    seen_c = c; seen_d = d; seen_e = e; seen_f = f; seen_flag = flag; seen_desc = description;
    std::memcpy(desc_bytes, description, 72);
}
static short half(const unsigned char* p) { short h; std::memcpy(&h, p, 2); return h; }

int main() {
    alignas(8) unsigned char object[160], other[64];
    for (unsigned flags : {0u, 0x40u, 0x3fu, 0xffffffbfu, 0xffffffffu}) {
        for (int mode : {0, 1, 2, 3, 4, -2, -3, 130}) {
            for (int frame : {0, 12, 24, -12, 1, 11, 13, -1, 36}) {
                std::memset(object, 0, sizeof object);
                std::memset(other, 0, sizeof other);
                std::memcpy(other + 12, &flags, 4);
                object[138] = static_cast<unsigned char>(mode);
                frameNm = frame; calls = 0;
                msg_collision__5TorchP10GameObject(object, other);
                const bool run = (flags & 0x40u) && mode != 2 && mode != 3 && frame % 12 == 0;
                if (calls != (run ? 1 : 0)) return 1;
                if (!run) continue;
                if (seen_target != other || seen_source != object || seen_a != 0.0f || seen_b != 0.0f) return 2;
                if (seen_c != 10000000 || seen_d != 0 || seen_e != 0 || seen_f != 0x0100c040 || seen_flag) return 3;
                if (seen_desc == nullptr) return 4;
                if (desc_bytes[32] != 0xff || desc_bytes[33] != 0xff) return 5;
                for (int i = 0; i < 4; ++i) if (half(desc_bytes + 16 + 2 * i) != 0 || half(desc_bytes + 24 + 2 * i) != 0) return 6;
                if (half(desc_bytes + 36) != 21 || half(desc_bytes + 38) != 1 || half(desc_bytes + 40) != 1 || half(desc_bytes + 42) != 0) return 7;
                for (int k = 1; k < 4; ++k) if (half(desc_bytes + 36 + 8 * k) != -1) return 8;
            }
        }
    }
    return 0;
}
