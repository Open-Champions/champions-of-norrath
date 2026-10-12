#include "storage.hpp"
// Observed cutCurrentScene at 0x0048432c in .sdata: the scene pointer cutStartScene publishes.
// Inferred: the stored width is the whole extent; the void* type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
void* cutCurrentScene = nullptr;
}
