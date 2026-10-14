// Observed GetNPCPosition__7VehicleP6Point3Ps at 0x002779d0, 84 bytes.
// Calls getSeatPosition__7VehicleiP6Point3 with seat 0 and the output point.
// Then stores, as a halfword at the third argument, the integer angle
// iatan2__Fff(-float(vehicle+1132), -float(vehicle+1128)). Nothing is
// returned. What those two vehicle floats mean is not established here.
// Nothing here was executed on the EE.
extern "C" void getSeatPosition__7VehicleiP6Point3(unsigned char* vehicle, int seat, void* point);
extern "C" short iatan2__Fff(float a, float b);

typedef float __attribute__((may_alias)) AliasedFloat;

extern "C" void GetNPCPosition__7VehicleP6Point3Ps(unsigned char* vehicle, void* point, short* yaw) {
    getSeatPosition__7VehicleiP6Point3(vehicle, 0, point);
    *yaw = iatan2__Fff(-*reinterpret_cast<AliasedFloat*>(vehicle + 1132),
                       -*reinterpret_cast<AliasedFloat*>(vehicle + 1128));
}
