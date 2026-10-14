// Observed: Init__10VIMateriali at 0x010ce568, 240 bytes. With the object as
// this and n the second argument; the result is zero.
//   Stores 19 at this + 12, n at this + 8, zero in each of the four bytes at
//   this + 16..19 and 1 at this + 32. Then, for each layer i from 0 while
//   fewer than n layers have been done (nothing is done when n <= 0), with
//   r = this + 48 + 144 * i:
//     word r + 0 = -1; word r + 4 = 1; the four bytes at r + 8..11 = 0xff;
//     Identity__10VIMatrix33(r + 16) (nine floats up to r + 52);
//     word r + 12 = 0; float r + 52 = 0.79999995 (bits 0x3f4ccccc);
//     floats r + 56, r + 60, r + 64 and the word r + 68 = 0; word r + 72 = -1.
// Inferred: r + 4 holds the layer flags (SetLayerModulate and SetLayerFillType
// edit bits 0 and 5 of it), r + 12 the blend mode (SetLayerBlendMode), r + 52
// the texture LOD bias (SetLayerTextureLODBias), and this + 8 the layer count.
// The LOD bias is stored as its bit pattern: the reviewed EE compiler turns the
// decimal literal 0.79999995f into a different last bit than the image holds.
// Unknown: the other fields' names.
extern "C" {
void Identity__10VIMatrix33(void* matrix);

int Init__10VIMateriali(void* object, int layers) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 12) = 19;
    *reinterpret_cast<int*>(bytes + 8) = layers;
    bytes[19] = 0;
    bytes[16] = 0;
    bytes[17] = 0;
    bytes[18] = 0;
    *reinterpret_cast<int*>(bytes + 32) = 1;
    for (int i = 0; i < layers; ++i) {
        unsigned char* record = bytes + 48 + 144 * i;
        *reinterpret_cast<int*>(record) = -1;
        *reinterpret_cast<int*>(record + 4) = 1;
        record[8] = 0xff;
        record[9] = 0xff;
        record[10] = 0xff;
        record[11] = 0xff;
        Identity__10VIMatrix33(record + 16);
        *reinterpret_cast<int*>(record + 12) = 0;
        *reinterpret_cast<unsigned*>(record + 52) = 0x3f4ccccc;
        *reinterpret_cast<float*>(record + 56) = 0.0f;
        *reinterpret_cast<float*>(record + 60) = 0.0f;
        *reinterpret_cast<float*>(record + 64) = 0.0f;
        *reinterpret_cast<int*>(record + 68) = 0;
        *reinterpret_cast<int*>(record + 72) = -1;
    }
    return 0;
}
}
