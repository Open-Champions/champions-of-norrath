// Observed cheatMode at 0x00484540 (.sdata). The image bytes are zero. One word load and one word store use it.
// Inferred: only the access width. Unknown: the meaning of the value and its writers.
extern "C" int cheatMode = 0;
