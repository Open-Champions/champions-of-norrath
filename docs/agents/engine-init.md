# engine-init

Package notes for `engineInit__Ff` and `_engineCheckStack__FPCci` on the pinned SLUS-20565 image. Addresses were checked against that executable. Nothing here was executed on the EE.

## `_engineCheckStack__FPCci`

Observed at 0x00191310, 144 bytes. It reads 5120 bytes from 0x01fc0000, which is the address of `_stack`. Each byte is compared with 0xA5. A mismatch calls `scePrintf` with the text at 0x00464328 (`stack error!  %s:%d\n`, two spaces) and the two arguments, then stores zero at address 0. That text has no ELF symbol. The store is not a branch; the scan continues if it returns. Fresh EE compilation is not an original-byte match. The host contract covers an intact canary, a mismatch at the first byte, and two later mismatches with fall-through.

```sh
c++ -std=c++17 -O2 -fsanitize=address,undefined -Wall -Wextra -Werror \
  tools/checks/engine_check_stack_contract.cpp -o /tmp/engine-check-stack
/tmp/engine-check-stack
```

## Build-plan entries needed

```json
{
  "header_closure": [
    "sources/engine-init-startup/ee_mmio.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "check_stack.o",
  "profile": "authored_cpp",
  "source": "sources/engine-init-startup/check_stack.cpp"
}
```
