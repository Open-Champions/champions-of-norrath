// Observed dlgDir at 0x0048435c (.sdata). The image bytes are zero. Thirteen word loads and one word store use it; the word is treated as a pointer by its callers in the authored sources.
// Inferred: only the access width. Unknown: the meaning of the value and its writers.
extern "C" char* dlgDir = nullptr;
