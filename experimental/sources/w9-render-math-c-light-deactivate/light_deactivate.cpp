// Observed: the original function at 0x0013eb28 (symbol lightDeactivate__FP5Light, 52 bytes) is the
// target of the jal inside the Savepoint constructor, which is the call the placeholder
// name norrath_light_deactivate stands for. This file defines that placeholder name.
// The original-named definition lives in w8-fx-particles-light-deactivate.
// A light node holds two words: offset 0 is the next link and offset 4 is the
// previous link. Deactivation unlinks the node from that doubly linked chain:
// when a previous node exists its next link takes this node's next link, when a
// next node exists its previous link takes this node's previous link, then both
// of this node's links are cleared. Nothing is returned.
// Inferred: a chain of active lights. Unknown: the rest of the Light layout.
struct LightLinks {
    LightLinks* next;
    LightLinks* previous;
};

extern "C" void norrath_light_deactivate(LightLinks* light) {
    if (light->previous) light->previous->next = light->next;
    if (light->next) light->next->previous = light->previous;
    light->next = nullptr;
    light->previous = nullptr;
}
