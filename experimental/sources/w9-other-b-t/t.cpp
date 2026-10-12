// Observed: the global `t` at 0x00484c80 is one 32-bit word in initialized data (symbol size 4). The file
// word is 0. frontendInitControllerConfig__Fi stores a zero word to it; frontendRunControllerConfig__Fv
// loads it as a single-precision float (lwc1). A second, file-local symbol also named t exists at
// 0x004844e8 and is used by menuRun__FP4MenuP3padPi and menuDraw; it is not this one.
// Inferred: a float animation position of the controller configuration screen, as the existing authored
// frontendRunControllerConfig describes. The initial zero is the same bits as 0.0f.
// Unknown: the intended name; `t` is what the image's symbol table says.

extern "C" float t = 0.0f;
static_assert(sizeof(t) == 4, "t");
