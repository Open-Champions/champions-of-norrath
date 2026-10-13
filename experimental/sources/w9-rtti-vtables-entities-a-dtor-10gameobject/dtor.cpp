// Observed _$_10GameObject at 0x00141658, 680 bytes. The routine is the one already authored in
// gameobject-base-dtor under the name norrath_vt_dtor_10GameObject: store the GameObject vtable at
// object+72; repeatedly pick the next "name=" argument of the argument list at object+44
// (tag folded to lower case through the ctype view), pass the text after '=' to objectRemoveString;
// then listDestroy on object+8, object+4 and the object; bit 0 of the flag selects blockFree.
// This unit compiles that same source under the symbol the image and its callers use,
// by giving the function the original's label before the source is included. It adds no second
// implementation. Unknown: nothing beyond what the included source records.
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) asm("_$_10GameObject");
#include "../gameobject-base-dtor/dtor.cpp"
