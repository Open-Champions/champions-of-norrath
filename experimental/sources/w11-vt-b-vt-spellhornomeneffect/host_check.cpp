// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null slots observed in the image.
#include "vt_spellhornomeneffect.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf19SpellHornOmenEffect"; }
extern "C" void slot_fn_1() { hit = "_$_19SpellHornOmenEffect"; }
extern "C" void slot_fn_2() { hit = "run__19SpellHornOmenEffectG6Point3s"; }
extern "C" void slot_fn_3() { hit = "draw__5Spell"; }
extern "C" void slot_fn_4() { hit = "canQueue__5Spell"; }

static const char* const expected[6] = {
    "__tf19SpellHornOmenEffect",
    "_$_19SpellHornOmenEffect",
    "run__19SpellHornOmenEffectG6Point3s",
    "draw__5Spell",
    "canQueue__5Spell",
    nullptr,
};

int main() {
    if (sizeof(_vt_19SpellHornOmenEffect) / sizeof(_vt_19SpellHornOmenEffect[0]) != 6) return 1;
    for (int i = 0; i < 6; ++i) {
        if (_vt_19SpellHornOmenEffect[i].adjustment != 0) return 2;
        if (expected[i] == nullptr) {
            if (_vt_19SpellHornOmenEffect[i].function != nullptr) return 4;
            continue;
        }
        hit = "";
        _vt_19SpellHornOmenEffect[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 3;
    }
    return 0;
}
