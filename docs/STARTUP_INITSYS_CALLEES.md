# InitSys callees

`main` and `launchSRS` do not reference these symbols. They are the startup calls and data opened by the nine `_InitSys` services. None of them is `engineAddTask`. Fresh compilation of the functions below reproduces the original instruction bytes except the three timer tail calls, whose `j` is a relocation to `SetT2`. No kernel service has run.

## Syscall requests

Each of these is four instructions: load the service number into `v1`, `syscall`, return through `ra`, with a no-op delay. The argument registers are not written. What the kernel does with the service is unknown.

| Symbol | Address | Service |
| --- | --- | --- |
| `AddIntcHandler2` | 0x002d4dd0 | 16 |
| `CreateThread` | 0x002d4ee0 | 32 |
| `StartThread` | 0x002d4f00 | 34 |
| `ChangeThreadPriority` | 0x002d4f70 | 41 |
| `GetThreadId` | 0x002d4fd0 | 47 |
| `FindAddress` | 0x002dd790 | 131 |

`FindAddress` is not `kFindAddress`.

## Interrupt mask

`DIntr` at 0x002dd640 is 84 bytes. It reads COP0 Status. When bit 16 is already clear it returns 0. Otherwise it returns 1 after `di` and `sync.p` have been repeated until bit 16 reads clear.

`EIntr` at 0x002dd698 is 24 bytes. It samples Status bit 16, executes `ei`, and returns 1 when that bit was already set.

## Timer 2

`SetT2` at 0x002ddea0 is 112 bytes. When Status bit 16 is set it disables interrupts and waits until the bit reads clear. It then sets Status bit 2, clears bit 1, and puts the saved bit 16 back into the status word. The value in `a1` is stored through `a0`. `ra` is written to COP0 ErrorEPC and `eret` returns to that address. The `jr ra` after `eret` is in the original body. Which exception level those status bits select, and whether a caller resumes after `eret`, is unknown.

`SetT2_COUNT`, `SetT2_MODE`, and `SetT2_COMP` are 16 bytes at 0x002ddf10, 0x002ddf20, and 0x002ddf30. Each moves its argument to `a1`, sets `a0` to `0xb0001000`, `0xb0001010`, or `0xb0001020`, and tail-calls `SetT2`. The original jump word names that address directly. These objects relocate the jump to the `SetT2` symbol.
