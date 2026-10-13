// maxDistance: a four-byte integer global, written by the shop and inventory grid code.
//
// Observed: the private image has an object symbol maxDistance at 0x00484968, four bytes long, in the
// initialized small-data section (.sdata). Its four file bytes are zero.
// Inferred: the initial value is zero. The shop and inventory grid unit assigns zero to it on one path;
// no other use is established here.
// Unknown: the section placement chosen by this build differs from the image's small-data placement, and
// no address is claimed for the linked object.
extern "C" {
int maxDistance = 0;
}
