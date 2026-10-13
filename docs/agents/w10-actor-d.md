# w10-actor-d: spell, skill and actor virtuals (Chest to Cthulu)

Branch `agents/w10-actor-d`. Addresses and sizes are from the pinned private `SLUS_205.65` `.text`. Each function below is its own commit under `experimental/sources/w10-actor-d-*`, has a host check beside it (`host_check.cpp`, not a plan input), and was compared against the image in a private R5900 interpreter harness (outside git): original and authored object run from identical memory snapshots with identical stubbed callees, comparing call logs (arguments and, where useful, an object checksum at each call) and every changed byte. Nothing here has run on the EE or as gameplay. Compiled sizes are the reviewed EE g++ `authored_cpp` objects; no byte-equality claim is made.

## Process note

Commit `d2327b6f` (message "x") contains only `experimental/sources/w10-actor-d-common/words.hpp`. It was pushed by mistake from a helper script; the header is used by later commits and its content is correct. History was not rewritten.

(Work in progress; the closing sections are filled in at the end of the run.)
