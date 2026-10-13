// Observed ModifyRPM__5Wheelf at 0x00278458, 8 bytes: stores the float argument at wheel+364 and nothing
// else. do_differential (w10-camveh-a) scales the value at +364 by a ratio and writes it back through this;
// SetNaturalRPM__5Wheel writes the same field. Unknown: the field's unit beyond the call sites' "RPM".
// Nothing here ran on the EE.
extern "C" void ModifyRPM__5Wheelf(void* self, float rpm) {
    *reinterpret_cast<float __attribute__((may_alias))*>(static_cast<unsigned char*>(self) + 364) = rpm;
}
