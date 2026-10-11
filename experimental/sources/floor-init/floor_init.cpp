/* floorInit at 0x00192f00, 56 bytes. It calls floorClear, then
 * engineAddTask(floorDraw, 28, the task label, 1). The label is unnamed
 * .rodata at 0x00464400: "drawFloorSprites (must be before drawworld)" and
 * its NUL. The scheduler and the drawing routine stay outside this file.
 * The authored clear symbol is the existing norrath_floor_clear.
 */
extern "C" {
void norrath_floor_clear();
void floorDraw__Fv();
void engineAddTask__FPFv_viPci(void (*callback)(), int order, char* label, int mode);

char norrath_floor_draw_task_label[] = "drawFloorSprites (must be before drawworld)";

void floorInit__Fv() {
    norrath_floor_clear();
    engineAddTask__FPFv_viPci(floorDraw__Fv, 28, norrath_floor_draw_task_label, 1);
}
}
