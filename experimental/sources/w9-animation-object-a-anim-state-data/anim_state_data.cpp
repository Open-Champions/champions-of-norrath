// Zero-initialised animation storage, extents taken from the private image's symbol table.
//   animState        264 bytes. Four unit-local objects share this ELF name (at 0x007cc5c8,
//                    0x007ccad8, 0x007d0440 and 0x007d0910); the one the shop start code
//                    points animStatePtr at is the single 264-byte AnimationState at
//                    0x007ccad8, whose next symbol (shopList) begins 264 bytes later. The
//                    other three are not defined here.
//   animStatePtr     0x00486aa4, 4 bytes, BSS: a pointer to the current animation state.
//   animUseBuffer    0x0048501f, 1 byte, file zero: a signed byte used as a row index by
//                    the animation initialiser.
//   animationLoading 0x00484358, 1 byte, file zero: a flag the dialog animation loader sets
//                    and clears.
//   animationLoadPos 0x00486a08, 4 bytes, BSS: the position the dialog animation loader is
//                    at (compared with a node count).
// Under the reviewed -G0 profile the file-backed zero bytes land in BSS; their values are
// all zero either way.
// Unknown: the declared types (the widths and signedness are from the importers' uses), and
// who else shares the other three animState objects' storage.
extern "C" {
unsigned char animState[264];
void* animStatePtr;
signed char animUseBuffer;
unsigned char animationLoading;
int animationLoadPos;
}
