// Models the original single-threaded word update modulo 2^32.
// This is not an atomic operation or a reconstruction of the other state users.
typedef char unsigned_width[sizeof(unsigned) == 4 ? 1 : -1];
extern "C" unsigned norrath_interface_mode;
extern "C" void norrath_light_disable_local_lights()
{
    norrath_interface_mode += 10u;
}
