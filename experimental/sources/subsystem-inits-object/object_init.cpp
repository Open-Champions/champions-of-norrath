// objectInit__Fv at 0x00140a80, 228 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No object has been drawn
// on the EE.
//
// engineAddTask__FPFv_viPci is called with runObjectsTask__Fv, 10, "runObjects",
// and 0, then with drawObjectsTask__Fv, 21, "drawObjects", and 1. Those names
// are the bytes at 0x00461e40 and 0x00461e30. listDestroy__FP8ListHead then
// receives a word of zero. objectGrid and ghostObjectGrid, 120 by 120 words
// each, are stored as zero, then offTheGrid and objectStrings, then the 512
// words of stringHash and objectHash.
extern "C" {
void runObjectsTask__Fv();
void drawObjectsTask__Fv();
void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag);
void listDestroy__FP8ListHead(void* head);

extern unsigned objectGrid[];
extern unsigned ghostObjectGrid[];
extern unsigned stringHash[];
extern unsigned objectHash[];
extern unsigned offTheGrid;
extern unsigned objectStrings;

static char norrath_run_objects_name[] = "runObjects";
static char norrath_draw_objects_name[] = "drawObjects";

void objectInit__Fv() {
    engineAddTask__FPFv_viPci(runObjectsTask__Fv, 10, norrath_run_objects_name, 0);
    engineAddTask__FPFv_viPci(drawObjectsTask__Fv, 21, norrath_draw_objects_name, 1);
    unsigned head = 0;
    listDestroy__FP8ListHead(&head);

    for (int cell = 0; cell < 120 * 120; ++cell) {
        objectGrid[cell] = 0;
        ghostObjectGrid[cell] = 0;
    }
    offTheGrid = 0;
    objectStrings = 0;
    for (int slot = 0; slot < 512; ++slot) {
        stringHash[slot] = 0;
        objectHash[slot] = 0;
    }
}
}
