// Host contract for _vt$6Sparks; not an EE input. Includes the source with every slot function
// defined as a distinct stub, then checks that slot i holds stub i, that every adjustment word is
// zero and that the last slot is empty.
#include "vtable.cpp"
#include <cstdio>

extern "C" {
void slot_fn_0() {} void slot_fn_1() {} void slot_fn_2() {} void slot_fn_3() {} void slot_fn_4() {} void slot_fn_5() {}
void slot_fn_6() {} void slot_fn_7() {} void slot_fn_8() {} void slot_fn_9() {} void slot_fn_10() {} void slot_fn_11() {}
void slot_fn_12() {} void slot_fn_13() {} void slot_fn_14() {} void slot_fn_15() {} void slot_fn_16() {}
}
int main() {
    void (*const stubs[17])() = {slot_fn_0, slot_fn_1, slot_fn_2, slot_fn_3, slot_fn_4, slot_fn_5, slot_fn_6, slot_fn_7, slot_fn_8,
                                 slot_fn_9, slot_fn_10, slot_fn_11, slot_fn_12, slot_fn_13, slot_fn_14, slot_fn_15, slot_fn_16};
    int bad = 0;
    for (int i = 0; i < 17; ++i) {
        if (vt_6Sparks[i].function != stubs[i]) { std::printf("fail slot %d function\n", i); bad = 1; }
        if (vt_6Sparks[i].adjustment != 0) { std::printf("fail slot %d adjustment\n", i); bad = 1; }
    }
    if (vt_6Sparks[17].function != nullptr || vt_6Sparks[17].adjustment != 0) { std::puts("fail end marker"); bad = 1; }
    if (sizeof(vt_6Sparks) / sizeof(vt_6Sparks[0]) != 18) { std::puts("fail count"); bad = 1; }
    return bad;
}
