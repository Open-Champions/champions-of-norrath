// Observed invHeader at 0x004848e0, 4 bytes, section .sdata/.data (D).
// One initialised word of 4 bytes whose file bytes are zero. invInitSharedInterfaceTextures__Fv stores it; inventoryFrameDraw__Fv and shopDrawTaskBar__FP12screenStruct load it.
// Inferred: A 32-bit address word (other units declare it void*), zero at start.
// Unknown: What it points at; the name is the one the image carries.
extern "C" {
unsigned invHeader = 0;
}
