// Observed: Init__10VIWndLegal at 0x010bf978 is 8 bytes: it returns 0 and touches nothing.
// The only argument seen by the image is the object. Unknown: whether the original signature
// carried further arguments that this body ignores (callers in the image pass only the object
// in the setup initialiser).
extern "C" int Init__10VIWndLegal(void*) { return 0; }
