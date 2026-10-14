// Naming bridge, not a reconstruction. The constructor sources import the animation
// lookup as norrath_object_find_animation; the routine itself is the already authored
// objectFindAnimation__FP10GameObjectPc (original at 0x152cc8, w8-object-list-math
// find-animation unit). A symbol alias cannot cross translation units in the partial
// build, so this one-instruction forwarder gives the imported name a definition. It
// passes both arguments through and returns the result unchanged. It corresponds to
// no original function and should be dropped when the importers use the original name.
extern "C" void* objectFindAnimation__FP10GameObjectPc(void* object, char* name);

extern "C" void* norrath_object_find_animation(void* object, char* name) {
    return objectFindAnimation__FP10GameObjectPc(object, name);
}
