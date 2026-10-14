// Observed msg_poke__7Vehiclei at 0x2779b0, extent 28: both arguments pass
// through unchanged to addPassenger__7Vehiclei and its result is the result
// here. Nothing else is read or stored.
extern "C" unsigned addPassenger__7Vehiclei(void* vehicle, int seat);

extern "C" unsigned msg_poke__7Vehiclei(void* vehicle, int seat) {
    return addPassenger__7Vehiclei(vehicle, seat);
}
