# Kernel patch tables

Package work is on `agents/kernel-patches`. Sources are under `experimental/sources/kernel-patches-*`. The syscall wrappers and the two pattern bodies were assembled with the reviewed `authored_entry_asm` flags and compared byte for byte with `experimental/sources/gs-dma-kernel-body-check/host_check.cpp` against the local executable image. The other functions were compiled with the `authored_cpp` flags and are checked by the `host_contract.cpp` beside each one. The data objects are checked by the Python tests in `host-tests/kernel-patches/`, which compile each source with the EE compiler and compare size, section, bytes, and relocation targets. `host_contract.cpp`, the body checker, and the Python tests are not build-plan inputs. `.gitignore` got three lines to allow `host-tests/kernel-patches/*.py`.

All addresses are from the pinned local executable. No kernel service, interrupt, or timer register was executed; every check below is a host model or a byte comparison.

## How the checks run

Byte comparison, one symbol at a time (the object name and address are the table values below):

```sh
c++ -std=c++17 -O1 experimental/sources/gs-dma-kernel-body-check/host_check.cpp -o work/host_check
EE=/path/to/reviewed/ee/bin/mips64r5900el-ps2-elf-g++
"$EE" -G0 -O2 -ffreestanding -fno-asynchronous-unwind-tables -fno-pic -fno-stack-protector -fno-unwind-tables -fomit-frame-pointer -fwrapv -mabi=eabi -march=r5900 -mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float -nostdinc -x assembler-with-cpp -c <source>.S -o work/unit.o
work/host_check work/private/SLUS_205.65 <address> work/unit.o <symbol>
```

Host contract for a function (shown for `topThread`; every function directory has one):

```sh
c++ -std=c++17 -Wall -Wextra -Werror -fsanitize=address,undefined experimental/sources/kernel-patches-top-thread/host_contract.cpp -o work/top-thread && work/top-thread
```

Data objects:

```sh
cd host-tests/kernel-patches && python3 -I -m unittest discover -s . -p 'test_*.py'
```

Each host contract was also run against at least one deliberately broken copy of its source (a changed constant, comparison, or shift) and failed. The byte comparer fails on a wrong address.

## Functions

| Symbol | Original address | Original size | Form |
| --- | --- | --- | --- |
| `SignalSema` | `0x002d5100` | 16 | service 66, byte match |
| `kernel_syscall_table_setup` | `0x002dd8d8` | 16 | service 116, byte match |
| `kernel_execps2_setup` | `0x002dd940` | 16 | service 116, byte match |
| `kernel_execps2_copy` | `0x002dd950` | 16 | service 90, byte match |
| `kernel_execps2_get_entry` | `0x002dd998` | 16 | service 91, byte match |
| `kernel_alarm_setup` | `0x002ddd60` | 16 | service 116, byte match |
| `kernel_alarm_copy` | `0x002ddd70` | 16 | service 90, byte match |
| `kernel_alarm_get_entry` | `0x002dddb8` | 16 | service 91, byte match |
| `kernel_tlb_copy` | `0x002dfb78` | 16 | service 90, byte match |
| `kernel_tlb_get_entry` | `0x002dfbc0` | 16 | service 91, byte match |
| `kernel_tlb_setup` | `0x002dfbd0` | 16 | service 116, byte match |
| `kernel_syscall_pattern_kcopy` | `0x002dd718` | 56 | assembly, byte match |
| `kFindAddress` | `0x002dd750` | 64 | assembly, byte match |
| `kernel_execps2_pattern_kcopy` | `0x002dd960` | 56 | assembly, byte match (added name) |
| `kernel_alarm_pattern_kcopy` | `0x002ddd80` | 56 | assembly, byte match (added name) |
| `kernel_tlb_pattern_kcopy` | `0x002dfb88` | 56 | assembly, byte match (added name) |
| `iGetTimerSystemTime` | `0x002de780` | 76 | host contract, compiles to 80 |
| `_InitAlarm` | `0x002df208` | 76 | host contract, compiles to 52 |
| `PatchIsNeeded` | `0x002dd9a8` | 104 | host contract, compiles to 88 |
| `topThread` | `0x002d5de0` | 216 | host contract, compiles to 224 |
| `SetNextComp` | `0x002de290` | 400 | host contract, compiles to 316 |
| `cbTimerHandler` | `0x002de4e0` | 672 | host contract, compiles to 672 (bytes differ) |
| `UnlinkAlarm` | `0x002de4a8` | 56 | host contract, compiles to 52 (added) |
| `InsertAlarm` | `0x002de420` | 136 | host contract, compiles to 160 (added) |

The compiled C++ bodies are not original-byte matches and are not claimed as such.

## Syscall requests

The eleven 16-byte wrappers are four instructions each: load the service number into `v1`, `syscall`, return through `ra` with a no-op delay. Argument registers and the kernel result stay in their incoming registers. The original has several local functions that share the spellings `setup`, `Copy`, `GetEntryAddress`, and `kCopy`; each copy that `InitSystemCallTableAddress`, `InitExecPS2`, `InitAlarm`, and `InitTLBFunctions` call is its own function here, under the import names those sources already use. Services 116, 90, and 91 are the setup, copy, and entry-address requests of those copies. What the kernel does with them is unknown.

## Pattern bodies

`kFindAddress` (64 bytes) and the four `kCopy` copies (56 bytes each) are not only called. `InitSystemCallTableAddress` passes the first two to `FindAddress(0x80000000, 0x80080000, pattern)` as byte patterns to look for in kernel memory, and the four SysEntry tables store a `kCopy` address as the handler for service 90. A compiled equivalent would not do: the instruction bytes themselves are the data. They are therefore authored assembly with the observed encodings, including the delay-slot filler, the alignment no-op, and branch-likely forms. All five compare equal to the executable. The four `kCopy` bodies are identical to each other; the tables point at distinct copies, so there are four definitions. The three names `kernel_execps2_pattern_kcopy`, `kernel_alarm_pattern_kcopy`, and `kernel_tlb_pattern_kcopy` extend the import name `kernel_syscall_pattern_kcopy` and are not in the original list.

`kFindAddress` compares the word at the start address; while the cursor is below the end it advances by four and compares again. It returns the address of the first equal word, else 0. Its result is cleared by `movz` on the word just compared, so a hit on a word that is zero also returns 0. `kCopy` copies `count >> 2` words and returns 0.

## Timer and alarm functions

`g_Timer` (0x004459a8, 32 bytes) is a record read as: a 64-bit overflow count at offset 0; the interrupt handler number at 8, initially -1; a word at 12 that is 1 and never written by these sources; offset 16, cleared by `InitTimer`; offset 20, the head of the pool chain `InitTimer` builds through `g_CounterBuf`; offset 24, the first element of the alarm chain; offset 28, an identifier stored while callbacks run and -1 otherwise. These offsets are labels, not original field names. An alarm record, as seen from `InsertAlarm`, `UnlinkAlarm`, `SetNextComp`, and `cbTimerHandler`, has next at 0, previous at 4, words at 8 and 12, 64-bit words at 16, 24 and 32, a function address at 40, a word at 44 that `cbTimerHandler` loads into `gp`, and a word at 48. The time of an alarm is the word at 32 plus the word at 16 minus the word at 24.

`iGetTimerSystemTime` reads the count word at `0x10001000`, the mode word at `0x10001010`, and the overflow word, re-reads the count when mode bit 0x800 is set, adds 1 to its local copy, and returns the 64-bit value `((overflow << 16) | count)` shifted left by 0, 4, 8, or 16 for mode low bits 0 to 3. `SetNextComp` and `cbTimerHandler` shift by four times the low bits instead (0, 4, 8, 12). The two disagree only for low bits 3; the reason is unknown.

`SetNextComp` takes a 64-bit time. It returns when `g_Timer` offset 28 is nonnegative. With no alarm it sets the compare to 0 and calls `SetT2_MODE` with 0x800 cleared. Otherwise it takes the first alarm's time and keeps moving to the next alarm while that alarm's time is below the current time plus 29491, then chooses between a short delay (count plus 29491 shifted) and the shifted time itself, depending on whether the signed difference from the argument is below 29491.

`cbTimerHandler` is the handler `InitTimer` registers on cause 11. While the compare flag (0x400) is set and the first alarm is due, it unlinks the alarm, stores an identifier, loads `gp` from the alarm, calls the alarm function with five arguments (identifier, the word at 32, elapsed time, the word at 48, and the handler's own third argument), then handles a 64-bit result: 0 clears bit 2 of the word at 12, all ones returns the alarm to the offset-20 pool, anything else adds the result, at least 14745, to the word at 32 and re-inserts the alarm. It ends by storing -1 at offset 28, calling `SetNextComp` with a fresh time, doing two overflow checks, `sync`, `ei`, and returning 0. The alarm call and the `gp` change are done in the host model only through a hook; on the EE the source uses an inline `move $28` and an indirect call. `gp` is not restored, as in the original. The compiled form loads `gp` before the call; whether the kernel's interrupt return restores it is unknown.

`StartTimerSystemTime` in `runtime-initsys` declares `iGetTimerSystemTime` as returning `unsigned` and `SetNextComp` as taking `unsigned`. The compiled caller passes `v0` straight into `a0` (`move a0,v0`), so the full 64-bit register reaches `SetNextComp`; the declarations do not truncate it.

`_InitAlarm` is not `InitAlarm`. It chains 64 sixteen-byte entries of `g_AlarmBuf` through their first words and sets `g_pFreeAlarm`. `UnlinkAlarm` and `InsertAlarm` are the removal and sorted insertion for the alarm chain at `g_Timer` offset 24. They were not in the original list; they are authored here because `cbTimerHandler` calls them and no other package defines them.

`PatchIsNeeded` writes the OSD configuration word with bits 13 to 15 set to 1, reads it back, restores the original, and returns 1 when those bits read back as zero. The meaning of the configuration word is unknown.

`topThread` is the body of the thread `InitThread` starts. It waits on `topSema`, takes the word at offset 0 of `topArg` masked with 511 as an index, stores index plus 1 back, reads a two-byte request at offset 8 plus twice the index, and calls `WakeupThread` (kind 0), `RotateThreadReadyQueue` (kind 1), `SuspendThread` (kind 2), or prints a fixed diagnostic through `kprintf` for any other kind. It is the consumer side of the ring `iWakeupThread` fills (see `sound-rpc-views`).

## Data

| Symbol | Address | Size | Content |
| --- | --- | --- | --- |
| `g_Timer` | `0x004459a8` | 32 | words 0, 0, -1, 1, 0, 0, 0, -1 |
| `topId` | `0x00444958` | 4 | zero |
| `topSema` | `0x008de0c0` | 4 | zero (symbol size 0; the next word is unread) |
| `topArg` | `0x008de0c8` | 1080 | zero (symbol size 0; extent is the gap to `q.12`) |
| `g_CounterBuf` | `0x008e2140` | 8192 | zero (symbol size 0; extent is the gap to `g_AlarmBuf`) |
| `g_SysRebootBuf` | `0x008e4548` | 512 | zero (symbol size 0; extent is the gap to `userarea`) |
| `g_pUserRebootBuf` | `0x004459c8` | 4 | zero |
| `g_iUserRebootSize` | `0x004459cc` | 4 | zero |
| `_SyscallEntry` | `0x00444a18` | 4 | zero |
| `_kExecArg` | `0x00445d48` | 4 | zero |
| `sifParamOrd` | `0x008e6bc0` | 48 | zero (symbol size 0; the memory-card calls pass it with a send size of 48) |
| `semaidRegFunc` | `0x00446074` | 4 | -1 |
| `semaid_timerfunc` | `0x00446078` | 4 | -1 |
| `kernel_syscall_sysentry` | `0x00444a20` | 16 | 0x83, address of `kFindAddress`, 0x5a, address of `kernel_syscall_pattern_kcopy` |
| `SysExecPS2Entry` | `0x004451e0` | 24 | 0x5a, address of `kernel_execps2_pattern_kcopy`, 0x5b, 0x80074000, 0xffffc402, 0 |
| `kernel_alarm_sysentry` | `0x00445968` | 64 | 0x5a, address of `kernel_alarm_pattern_kcopy`, 0x5b, 0x80076000, then pairs (0xfc,0) (0xfe,0) (0xfd,0) (0xff,0) (0x12c,0) (8,0) |
| `kernel_tlb_sysentry` | `0x00445d50` | 64 | 0x5a, address of `kernel_tlb_pattern_kcopy`, 0x5b, 0x80075000, 0x54, address of `_kExitTLBHandler`, then pairs (0x55,0) (0x56,0) (0x57,0) (0x58,0) (0x59,0) |
| `g_AlarmBuf` | `0x008e4140` | 1024 | zero (added; symbol size 0, gap to `g_pFreeAlarm`) |
| `g_pFreeAlarm` | `0x008e4540` | 4 | zero (added) |

The initialized contents were compared against the executable image with the relocation words masked; the three table address words resolve to the original addresses of the corresponding functions (`kFindAddress` `0x002dd750`, the four `kCopy` copies, and `_kExitTLBHandler` `0x002e0100`). No source here writes the SysEntry tables; the callers only read them. The original places the zero-valued words in `.data`; this `-G0` profile places zero-initialized objects in `.bss` and the `-1` and table words in `.data`. As with the vblank words in `gs-dma-kernel`, this does not reproduce the original addresses. The zero-valued entries show up as `.bss` in the object checks and nothing else depends on the section.

`osdsrc` (`0x00444a38`, 1960 bytes) and `tlbsrc` (`0x004459d0`, 816 bytes) are not authored. See the end of this file.

## Imports closed

`PatchIsNeeded`, `SetNextComp`, `SignalSema`, `SysExecPS2Entry`, `_InitAlarm`, `_SyscallEntry`, `_kExecArg`, `cbTimerHandler`, `g_CounterBuf`, `g_SysRebootBuf`, `g_Timer`, `g_iUserRebootSize`, `g_pUserRebootBuf`, `iGetTimerSystemTime`, `kFindAddress`, `kernel_alarm_copy`, `kernel_alarm_get_entry`, `kernel_alarm_setup`, `kernel_alarm_sysentry`, `kernel_execps2_copy`, `kernel_execps2_get_entry`, `kernel_execps2_setup`, `kernel_syscall_pattern_kcopy`, `kernel_syscall_sysentry`, `kernel_syscall_table_setup`, `kernel_tlb_copy`, `kernel_tlb_get_entry`, `kernel_tlb_setup`, `kernel_tlb_sysentry`, `semaidRegFunc`, `semaid_timerfunc`, `sifParamOrd`, `topArg`, `topId`, `topSema`, `topThread`. That is 36 of the 38.

This was measured by linking the 43 compiled units of this package into the shared branch's partial object (`ld -r`, no duplicate definitions): unresolved imports went from 1149 to 1120. Not closed: `osdsrc`, `tlbsrc`.

## Imports opened

`GetOsdConfigParam`, `SetOsdConfigParam` (called by `PatchIsNeeded`), `WakeupThread`, `RotateThreadReadyQueue`, `SuspendThread`, `kprintf` (called by `topThread`), `_kExitTLBHandler` (the address in `kernel_tlb_sysentry`). All are unresolved and are not defined anywhere in the repository's branches. The first six are called by name from the sources here; `_kExitTLBHandler` (`0x002e0100`, 244 bytes) is a global original function that the tlb table points at.

Also defined here but not in the original list of 38, closing imports that this package's code would otherwise open: `kernel_execps2_pattern_kcopy`, `kernel_alarm_pattern_kcopy`, `kernel_tlb_pattern_kcopy`, `InsertAlarm`, `UnlinkAlarm`, `g_AlarmBuf`, `g_pFreeAlarm`. No other branch referenced them.

`srcfile` and `eenull` (the alarm patch sources `InitAlarm` copies) were already unresolved and are still unresolved; they are not in this package's list.

## Build-plan entries needed

Append these to `experimental/build-plan.json` and the matching `emitted_functions` to `experimental/snapshot.json`. They continue from `input_order` 1257, the end of the plan on this branch; renumber if the integrator's plan has grown. `header_closure` is empty for all. Do not add the host contracts. The dependency order does not matter for linking.

```json
{"emitted_functions":[{"compiled_body_bytes":16,"name":"SignalSema"}],"header_closure":[],"input_order":1258,"kind":"independently_authored_source","object_name":"signal_sema.o","profile":"authored_entry_asm","sha256":"16b5a4dd3ad74d4c07bcd0695764dd1b9fd1a7a146685eb3e6eaa0b2b24d9ff4","source":"sources/kernel-patches-signal-sema/signal_sema.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_syscall_table_setup"}],"header_closure":[],"input_order":1259,"kind":"independently_authored_source","object_name":"syscall_table_setup.o","profile":"authored_entry_asm","sha256":"5999ac342b744bbae619510fa119ebbfb251023eb1b6e269badb6c7976a7b20d","source":"sources/kernel-patches-syscall-table-setup/syscall_table_setup.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_execps2_setup"}],"header_closure":[],"input_order":1260,"kind":"independently_authored_source","object_name":"execps2_setup.o","profile":"authored_entry_asm","sha256":"b53cd88032e49c46b626cf2f9dbd760a6e0ebe0d66f73f071133d7548230c590","source":"sources/kernel-patches-execps2-setup/execps2_setup.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_execps2_copy"}],"header_closure":[],"input_order":1261,"kind":"independently_authored_source","object_name":"execps2_copy.o","profile":"authored_entry_asm","sha256":"e3bed75ae05ecb41d36c4bddc4b2530ce05ccc6b847a1a53aab22d50d82d7de0","source":"sources/kernel-patches-execps2-copy/execps2_copy.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_execps2_get_entry"}],"header_closure":[],"input_order":1262,"kind":"independently_authored_source","object_name":"execps2_get_entry.o","profile":"authored_entry_asm","sha256":"0f4c1e72c5298298958ac20e3c193255f1aa7c8a149ba958ddaab22353c8f4eb","source":"sources/kernel-patches-execps2-get-entry/execps2_get_entry.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_alarm_setup"}],"header_closure":[],"input_order":1263,"kind":"independently_authored_source","object_name":"alarm_setup.o","profile":"authored_entry_asm","sha256":"91e4112daa910874014b020097f709d81b0db254aa2af273d2b89ac5c206b690","source":"sources/kernel-patches-alarm-setup/alarm_setup.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_alarm_copy"}],"header_closure":[],"input_order":1264,"kind":"independently_authored_source","object_name":"alarm_copy.o","profile":"authored_entry_asm","sha256":"005643a6d2ea3a8847b8e600b828dfca2d6a5e825b9bc408c10cf43fdc84b574","source":"sources/kernel-patches-alarm-copy/alarm_copy.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_alarm_get_entry"}],"header_closure":[],"input_order":1265,"kind":"independently_authored_source","object_name":"alarm_get_entry.o","profile":"authored_entry_asm","sha256":"78a99c4d3b6e6b078132d51af0c8cd23e9bf25f241b9adb814ee0b00706299e7","source":"sources/kernel-patches-alarm-get-entry/alarm_get_entry.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_tlb_copy"}],"header_closure":[],"input_order":1266,"kind":"independently_authored_source","object_name":"tlb_copy.o","profile":"authored_entry_asm","sha256":"91a13d2b5885590cad932bf1d7b07b1242f0d3f2e19c9d0c359b0bcd7aec7828","source":"sources/kernel-patches-tlb-copy/tlb_copy.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_tlb_get_entry"}],"header_closure":[],"input_order":1267,"kind":"independently_authored_source","object_name":"tlb_get_entry.o","profile":"authored_entry_asm","sha256":"28326c865ff5ab02782035ef29a6bc397e1ad19715fcbcc8a243d7c9121a64d6","source":"sources/kernel-patches-tlb-get-entry/tlb_get_entry.S"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"kernel_tlb_setup"}],"header_closure":[],"input_order":1268,"kind":"independently_authored_source","object_name":"tlb_setup.o","profile":"authored_entry_asm","sha256":"9647b9eff1c4747f809ff10dce27812bcab41c443f1b6d460f6c21d49ddc0c7d","source":"sources/kernel-patches-tlb-setup/tlb_setup.S"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"kernel_syscall_pattern_kcopy"}],"header_closure":[],"input_order":1269,"kind":"independently_authored_source","object_name":"syscall_pattern_kcopy.o","profile":"authored_entry_asm","sha256":"f64437d3d41bb7c40069a7af3a027360079730e5ede2163e17bc01aa3df68ed9","source":"sources/kernel-patches-syscall-pattern-kcopy/syscall_pattern_kcopy.S"}
{"emitted_functions":[{"compiled_body_bytes":64,"name":"kFindAddress"}],"header_closure":[],"input_order":1270,"kind":"independently_authored_source","object_name":"k_find_address.o","profile":"authored_entry_asm","sha256":"bb8394532715230d4d92f3db29dad844dbe8874ad76ea87bd4041aa9a9cf2140","source":"sources/kernel-patches-k-find-address/k_find_address.S"}
{"emitted_functions":[{"compiled_body_bytes":80,"name":"iGetTimerSystemTime"}],"header_closure":[],"input_order":1271,"kind":"independently_authored_source","object_name":"get_timer_system_time.o","profile":"authored_cpp","sha256":"8bcdd6c81ad2fd6dc5c497dd4b175bf6faa411d35e50663d4b14d73189ab077a","source":"sources/kernel-patches-get-timer-system-time/get_timer_system_time.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"_InitAlarm"}],"header_closure":[],"input_order":1272,"kind":"independently_authored_source","object_name":"init_alarm_list.o","profile":"authored_cpp","sha256":"975cd074c93c2ca7ba25a9dcca31a22d7bf5cfc2f2166735bbe9f305e075cd69","source":"sources/kernel-patches-init-alarm/init_alarm_list.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"PatchIsNeeded"}],"header_closure":[],"input_order":1273,"kind":"independently_authored_source","object_name":"patch_is_needed.o","profile":"authored_cpp","sha256":"6e3a4a681f8e85c9f7311a5218056ec4b5c21df5a32d1c2a30c0a8754b79241b","source":"sources/kernel-patches-patch-is-needed/patch_is_needed.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"topThread"}],"header_closure":[],"input_order":1274,"kind":"independently_authored_source","object_name":"top_thread.o","profile":"authored_cpp","sha256":"f2e289caf2768a237f913292558e86f7f226722978a7cdc6ab806f0343d1d0ff","source":"sources/kernel-patches-top-thread/top_thread.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"SetNextComp"}],"header_closure":[],"input_order":1275,"kind":"independently_authored_source","object_name":"set_next_comp.o","profile":"authored_cpp","sha256":"f63b823aeaf0d3aa69b70746db49d2dc60978aaf9695d9b7b8062bf76b458ec3","source":"sources/kernel-patches-set-next-comp/set_next_comp.cpp"}
{"emitted_functions":[{"compiled_body_bytes":672,"name":"cbTimerHandler"}],"header_closure":[],"input_order":1276,"kind":"independently_authored_source","object_name":"timer_handler.o","profile":"authored_cpp","sha256":"94f90685fff0e9d7e91afbc010391e7fb33c9ba21c8e970c784461df567c3b4f","source":"sources/kernel-patches-timer-handler/timer_handler.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"kernel_execps2_pattern_kcopy"}],"header_closure":[],"input_order":1277,"kind":"independently_authored_source","object_name":"execps2_pattern_kcopy.o","profile":"authored_entry_asm","sha256":"256513456b512ae4090b186cf1f6e5bffd0bf9c243128e717076874d4e4cf438","source":"sources/kernel-patches-execps2-pattern-kcopy/execps2_pattern_kcopy.S"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"kernel_alarm_pattern_kcopy"}],"header_closure":[],"input_order":1278,"kind":"independently_authored_source","object_name":"alarm_pattern_kcopy.o","profile":"authored_entry_asm","sha256":"a2873063d15ec0c6cea6e81f592a1fe57349110e8bb08d8dd4bd67da34689306","source":"sources/kernel-patches-alarm-pattern-kcopy/alarm_pattern_kcopy.S"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"kernel_tlb_pattern_kcopy"}],"header_closure":[],"input_order":1279,"kind":"independently_authored_source","object_name":"tlb_pattern_kcopy.o","profile":"authored_entry_asm","sha256":"9107748d90f80af7216fa2553d2cca0a21c31b7f298de6c3bbfca750b3307dd5","source":"sources/kernel-patches-tlb-pattern-kcopy/tlb_pattern_kcopy.S"}
{"emitted_functions":[],"header_closure":[],"input_order":1280,"kind":"independently_authored_source","object_name":"g_timer.o","profile":"authored_cpp","sha256":"fdbeefbf41e9bc0d735dda794bd461234f53f3e5c6fc6b0ed4bd60554dc6d74b","source":"sources/kernel-patches-g-timer/g_timer.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1281,"kind":"independently_authored_source","object_name":"top_id.o","profile":"authored_cpp","sha256":"ca6795607b1f6b211a50e8e621334247be57601c0cad8067fe08b445a1ff8340","source":"sources/kernel-patches-top-id/top_id.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1282,"kind":"independently_authored_source","object_name":"top_sema.o","profile":"authored_cpp","sha256":"5e5813e0e1bc36d19d573e0dbd88c02aa7a7e4bf200167ae64f53411cfb58ed0","source":"sources/kernel-patches-top-sema/top_sema.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1283,"kind":"independently_authored_source","object_name":"top_arg.o","profile":"authored_cpp","sha256":"abdd7e05dbe797382e11339dd938e82567e72a83efca9f558fbd1241004796f0","source":"sources/kernel-patches-top-arg/top_arg.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1284,"kind":"independently_authored_source","object_name":"g_counter_buf.o","profile":"authored_cpp","sha256":"8d9ad05a2197e1c15c4265fa5daa7803ff5f9343ce96fefc81043180d4681116","source":"sources/kernel-patches-g-counter-buf/g_counter_buf.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1285,"kind":"independently_authored_source","object_name":"g_sys_reboot_buf.o","profile":"authored_cpp","sha256":"8275b380438817885853cdcc12031f0c0bbe4e9c5d33c0f7f769168570cd4baa","source":"sources/kernel-patches-g-sys-reboot-buf/g_sys_reboot_buf.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1286,"kind":"independently_authored_source","object_name":"g_p_user_reboot_buf.o","profile":"authored_cpp","sha256":"ebdc137e75ca202b04c33f1f6df78d2ce6746c26af049393f1783bbca491fb00","source":"sources/kernel-patches-g-user-reboot-buf/g_p_user_reboot_buf.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1287,"kind":"independently_authored_source","object_name":"g_i_user_reboot_size.o","profile":"authored_cpp","sha256":"71749620ccedba1eabed7cd228064325f6a81ef54604fa35a2bc0f72196ffd3e","source":"sources/kernel-patches-g-user-reboot-size/g_i_user_reboot_size.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1288,"kind":"independently_authored_source","object_name":"syscall_entry.o","profile":"authored_cpp","sha256":"27671d8d0fb8d0f8c1832352f2ebfd26eda71eefe2ca0c5d2dd177b2b224b83d","source":"sources/kernel-patches-syscall-entry/syscall_entry.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1289,"kind":"independently_authored_source","object_name":"k_exec_arg.o","profile":"authored_cpp","sha256":"5a72e9485b20cb1ddceab5586cc4acdad159041069f1928b4268a02f3fb0fdb3","source":"sources/kernel-patches-k-exec-arg/k_exec_arg.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1290,"kind":"independently_authored_source","object_name":"sif_param_ord.o","profile":"authored_cpp","sha256":"aab7d320897bb5537a6b52a301c81f1da661fdfeb7f6cd6d26dc3bae1363e018","source":"sources/kernel-patches-sif-param-ord/sif_param_ord.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1291,"kind":"independently_authored_source","object_name":"semaid_reg_func.o","profile":"authored_cpp","sha256":"24d5aa6e20ea3bf7684b24daa29c5c05b360b73788a2e49165c5c1f77d5438cb","source":"sources/kernel-patches-semaid-reg-func/semaid_reg_func.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1292,"kind":"independently_authored_source","object_name":"semaid_timerfunc.o","profile":"authored_cpp","sha256":"e7bf298ba6d434d74076b059d9c3b0a3fda80dbb52a03be9b64e1bde4fae163b","source":"sources/kernel-patches-semaid-timerfunc/semaid_timerfunc.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1293,"kind":"independently_authored_source","object_name":"syscall_sysentry.o","profile":"authored_cpp","sha256":"bbaeab055a3bb00af0fa5fa1eec4e2f309b8daffbc8d3c4dddd79dac99dc3948","source":"sources/kernel-patches-syscall-sysentry/syscall_sysentry.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1294,"kind":"independently_authored_source","object_name":"exec_ps2_entry.o","profile":"authored_cpp","sha256":"daa849b8f45cf12f44f575dc33275b3a994b4d91f64a49be5188bf0e8d504656","source":"sources/kernel-patches-execps2-entry/exec_ps2_entry.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1295,"kind":"independently_authored_source","object_name":"alarm_sysentry.o","profile":"authored_cpp","sha256":"79d977340e8203eb4843e1f7cc6fc424735ff9a62425a24b4b4beb5446bc2098","source":"sources/kernel-patches-alarm-sysentry/alarm_sysentry.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1296,"kind":"independently_authored_source","object_name":"tlb_sysentry.o","profile":"authored_cpp","sha256":"c5a085803b0acd26a81a329e8a71404a5e6686a8b59e89b3c9d545980981ff83","source":"sources/kernel-patches-tlb-sysentry/tlb_sysentry.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"UnlinkAlarm"}],"header_closure":[],"input_order":1297,"kind":"independently_authored_source","object_name":"unlink_alarm.o","profile":"authored_cpp","sha256":"af6f4abd8b1f03be64d7db3e5abb906c36d082ad5a57e738d537e05495360374","source":"sources/kernel-patches-unlink-alarm/unlink_alarm.cpp"}
{"emitted_functions":[{"compiled_body_bytes":160,"name":"InsertAlarm"}],"header_closure":[],"input_order":1298,"kind":"independently_authored_source","object_name":"insert_alarm.o","profile":"authored_cpp","sha256":"f49c4ff2f777f8d7e50d838085cc39390a18a13104155f9882b398b89ff90977","source":"sources/kernel-patches-insert-alarm/insert_alarm.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1299,"kind":"independently_authored_source","object_name":"g_alarm_buf.o","profile":"authored_cpp","sha256":"7a92952303b47609c1cd3f95bc1fd6b31a3be02aca38bad6e2d4c338cc3f23ea","source":"sources/kernel-patches-g-alarm-buf/g_alarm_buf.cpp"}
{"emitted_functions":[],"header_closure":[],"input_order":1300,"kind":"independently_authored_source","object_name":"g_p_free_alarm.o","profile":"authored_cpp","sha256":"7816ee87c23f748d3b7cf6aa0db6754f003cd184be78c3ed34c42c05d7120417","source":"sources/kernel-patches-g-p-free-alarm/g_p_free_alarm.cpp"}
```

## Authored but never executed

Nothing here has run on the EE, an emulator, or the kernel. The byte comparisons show the assembly wrappers and the five pattern bodies equal the original instruction bytes; the C++ functions are modeled on the host only (fake timer registers, fake arena for the 32-bit alarm links, fake `WaitSema`/`SetT2_*`/callback hooks). In particular none of these has run: the service requests (66, 90, 91, 116), `kFindAddress` and `kCopy` as search patterns in kernel memory, `cbTimerHandler` as an interrupt handler (including the `gp` load, `sync`, and `ei`), `SetNextComp` against real compare behavior, the alarm and counter chains in the real `g_CounterBuf`, `topThread` as a kernel thread, and `PatchIsNeeded` against the real OSD configuration word. The compiled C++ differs from the original in instruction shape; for example `InsertAlarm` merges the two link-word stores into one 64-bit store.

## Unknown

- What services 90, 91, and 116 do inside the kernel, and what the SysEntry pairs mean beyond their position and values.
- Where in kernel memory the `kFindAddress` and `kCopy` patterns are found, and whether a scan terminates on a real image.
- The contents of `osdsrc` (1960 bytes copied to `0x80074000`) and `tlbsrc` (816 bytes copied to `0x80075000`). They are original machine code. Publishing their bytes, as data or as an assembly listing, would republish original code, so they are left unauthored and remain imports. Closing them needs an owner decision: link them from a private extraction at integration time, or reconstruct them as understood source. Their sizes, copy destinations, and consumers are established.
- What the word at `g_Timer` offset 12 (initially 1) means, and why the shift for mode low bits 3 differs between `iGetTimerSystemTime` (16) and the other two functions (12).
- The meaning of the alarm words at 8, 12, and 16 to 32, beyond what the code does with them.
- Whether `gp` is restored by the kernel's interrupt return after `cbTimerHandler`.
- The OSD configuration word layout used by `PatchIsNeeded`.
- Required alignment of `topArg`, `g_CounterBuf`, `g_AlarmBuf`, and `sifParamOrd`; the source uses 8, 64, 16, and 16 from the observed addresses and access widths.
- Whether any byte past the last request in `topArg` is used, and what the unread word between `topSema` and `topArg` is.
