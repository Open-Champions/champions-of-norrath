// statsDrawDelay: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source level):
// - Reference image: an initialised-data word of size 4 at 0x00484a34 (reviewed EE nm).
// - Caller: w15-gameloop-a-stats-frame/stats_frame.cpp decrements it while it is above zero, then returns.
// Inferred:
// - One 32-bit int, from the caller's declared type and the 4-byte symbol size.
// Unknown:
// - The initial value in the image. This unit supplies zero, so the first frames are not delayed.
// This unit does not establish any field value.
extern "C" {
int statsDrawDelay = 0;
}
static_assert(sizeof statsDrawDelay == 4u, "statsDrawDelay extent");
