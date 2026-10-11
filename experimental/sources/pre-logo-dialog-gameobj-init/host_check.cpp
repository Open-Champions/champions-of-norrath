// Host contract for gameobjInit. Not an EE plan input.
#include "gameobj_init.cpp"

static int create_sets;
static int filter_sets;
static void (*seen_create)();
static void (*seen_filter)();

extern "C" void createByName__FPcG6Point3sPPc() {}
extern "C" void neverSaveAlwaysLoadFromLump__FPc() {}

extern "C" void objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(void (*create)()) {
    ++create_sets;
    seen_create = create;
}

extern "C" void objectSetFilterFunction__FPFPc_i(void (*filter)()) {
    ++filter_sets;
    seen_filter = filter;
    if (create_sets != 1) filter_sets = -1;
}

int main() {
    gameobjInit__Fv();
    if (create_sets != 1 || filter_sets != 1) return 1;
    if (seen_create != createByName__FPcG6Point3sPPc) return 2;
    if (seen_filter != neverSaveAlwaysLoadFromLump__FPc) return 3;
    return 0;
}
