# w10-actor-b: spell, skill and actor virtuals (AntLanternCtors to AutoObject)

Branch `agents/w10-actor-b`. Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses below are that
file's symbol values; no game bytes or listings are committed. (Draft written mid-run; the
final summary at the end of this file supersedes it.)

Method for every function: compiled with the reviewed EE g++ `authored_cpp` flags (single unit,
not a full partial build); a host check under the unit directory (not an EE input); and a paired
run in a private R5900 interpreter (outside git) that executes the original function and the
authored one from identical memory snapshots with identical stand-in callees and compares call
logs and every changed byte outside the stack. Mutants of each source were run through the paired
test and detected unless noted. Nothing ran on real hardware or an emulator.
