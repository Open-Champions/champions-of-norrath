// Host contract for addpmlistptrscap__Fv. Not an EE plan input. The source uses pointer-sized
// cells, so the host's 8-byte cells work the same as the target's 4-byte cells.
#include "add_pm_list_ptrs_cap.cpp"

extern "C" {
__UINTPTR_TYPE__ pmlistptrs[0x100];
int pmlistptrsused;
}

static void load(const __UINTPTR_TYPE__* v, int n) {
    for (int i = 0; i < 0x100; i++) pmlistptrs[i] = 0xdead;
    for (int i = 0; i < n; i++) pmlistptrs[i] = v[i];
    pmlistptrsused = n;
}

int main() {
    {   // the new run [1 2] equals the earlier run [1 2]: the duplicate is dropped
        const __UINTPTR_TYPE__ v[] = {1, 2, 0, 1, 2};
        load(v, 5);
        __UINTPTR_TYPE__* r = (__UINTPTR_TYPE__*)addpmlistptrscap__Fv();
        if (r != pmlistptrs + 0 || pmlistptrsused != 3) return 1;
        if (pmlistptrs[5] != 0) return 2;  // the sentinel was written
    }
    {   // no earlier equal run: the run is kept, one sentinel cell added
        const __UINTPTR_TYPE__ v[] = {1, 2, 0, 3};
        load(v, 4);
        __UINTPTR_TYPE__* r = (__UINTPTR_TYPE__*)addpmlistptrscap__Fv();
        if (r != pmlistptrs + 3 || pmlistptrsused != 5) return 3;
    }
    {   // only run in the list
        const __UINTPTR_TYPE__ v[] = {7, 8, 9};
        load(v, 3);
        __UINTPTR_TYPE__* r = (__UINTPTR_TYPE__*)addpmlistptrscap__Fv();
        if (r != pmlistptrs + 0 || pmlistptrsused != 4) return 4;
    }
    {   // match with the second earlier run: result is that run's start
        const __UINTPTR_TYPE__ v[] = {5, 0, 1, 2, 0, 1, 2};
        load(v, 7);
        __UINTPTR_TYPE__* r = (__UINTPTR_TYPE__*)addpmlistptrscap__Fv();
        if (r != pmlistptrs + 2 || pmlistptrsused != 5) return 5;
    }
    {   // a prefix of an earlier run is not equal: [1 2] against [1 2 3 0]
        const __UINTPTR_TYPE__ v[] = {1, 2, 3, 0, 1, 2};
        load(v, 6);
        __UINTPTR_TYPE__* r = (__UINTPTR_TYPE__*)addpmlistptrscap__Fv();
        if (r != pmlistptrs + 4 || pmlistptrsused != 7) return 6;
    }
    return 0;
}
