// Zero-initialised lump pointers, extents from the private image's symbol table.
//   domeLump        0x00486b00, 4 bytes, BSS: the lump the dome drawing walks (null means
//                   there is no dome); draw code tests it and looks up model and texture
//                   entries inside it.
//   dialogLumpName  0x0048500c, 4 bytes, file zero: a text pointer the dialog setup stores
//                   its second argument in.
//   eqLumpHeader    0x004845a0, 4 bytes, file zero: a pointer to a lump header; the next
//                   symbol (wLumpHeader) begins 4 bytes later.
// Under the reviewed -G0 profile the file-backed zeros land in BSS; all start null.
// Unknown: the pointee type of eqLumpHeader and who writes it.
extern "C" {
void* domeLump;
char* dialogLumpName;
void* eqLumpHeader;
}
