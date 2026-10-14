# w16-lib-a: third-party library code (GSM, zlib, network stack, string formatting)

Branch `agents/w16-lib-a`. Reference image: the private `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`
as recorded by the earlier packages). Addresses and sizes are that image's own symbol values; nothing from it (bytes, disassembly, decompiler
output) is committed. Nothing here has run on the EE or in the game.

## Result

Of the 21 assigned imports, seven (`Gsm_LPC_Analysis`, `Gsm_Long_Term_Predictor`, `Gsm_RPE_Encoding`, `inflateInit2_`, `inflate_blocks`,
`inflate_blocks_free`, `inflate_blocks_reset`) were already authored by `w15-libs-eh-a` but have no entry in `experimental/build-plan.json`,
so they stay unresolved until the integrator adds the entries below. I did not author them again. The other fourteen are authored here, plus
one helper they opened (`StrcpyS`) and the wide `vsnprintf16`. A private build with the entries below added (plan outside git, see
"Build") resolved all 21 assigned names.

## Method and checks

- Each function was read privately, rewritten as C++ in my own words and compiled with the reviewed `authored_cpp` flags (EE GCC 15.2.0).
  Compiled sizes differ from the image; no byte match is claimed.
- **Paired runs** (private, outside git): the authored unit is linked at a fixed address with every undefined symbol bound to the original's
  address, then run next to the original function in an R5900 interpreter from identical memory snapshots with identical scripted callee
  hooks. A run passes only when callee call logs (arguments, relevant memory at each call, stack words) and every changed byte outside the
  stack agree, plus the return value when the function returns one. `HALT` counts as a failure. Scenario counts are in each commit message.
  Mutations of constants, offsets, flags, test directions, call order and widths were run against the paired scenarios for every unit.
- **Host checks**: every unit has `host_check.cpp` (not a build input; it includes host headers) that builds with the host compiler and
  fails when an offset, branch, call order or argument is wrong. The EE compile of each unit was also done on its own.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).

## Observed (by function; details are in each source header)

- `Close__11VIUDPSocket` (0x0102a510): logs, then `sceInetClose(this + 8, word@+4, word@+68, 0)` and sets `+68 = -1` unless `+68` is already `-1`; returns `-1` or the close result.
- `VIInitUSocketExApi__Fv` (0x01048d70): `sMessagePool = __wrap_malloc(0x8240)`, `ll_init` on `sFreeMessages` then `sUsedMessages`, sixteen `ll_add_tail` of 2084-byte nodes onto the free list.
- `sceLibnetRegisterHandler` (0x002e5ea8): `CallRpc(object, 30, buffer, 0, 4)`, `-542` on failure, else the reply word.
- `sceLibnetWaitGetAddress` (0x002e62e0): flag bit 2 selects `WaitGetAddressAutoUpIf` (0x002e6148) or `WaitGetAddressNoAutoUpIf` (0x002e61e0), both five-argument local functions.
- `sceInetAddress2String` (0x002e51d8): `MemcpyD` 16 bytes at word 1, `CallRpc` function 7 (send 20, receive capacity + 20), `StrcpyS` from word 5; `sceInetAddress2Name` (0x002e5278): same shape with function 18, request in words 0-8 (word 8 from the ninth argument when flag 0x80 is set), text from word 8. `StrcpyS` (0x002e5040) copies the text when the destination is non-null.
- `ezNetCtlUpInterface` (0x010bfe08): RPC function 5 with the index in word 0 of `m_buf`. `ezNetCtlSetCombination` (0x010bfc28): two strcpy into `m_buf` (+4, +260), RPC function 2 (292 bytes both ways), then two **unconditional** prints and the status word. `ezNetCnfGetProperties` (0x010bf0f0): same layout in the eznetcnf client with the properties address in word 0, and its own local `m_rpcCd`, `m_rpcBuf`, `m_rpcRunning` (0x01192c68).
- `hostname` (0x0118e6c0, 128 bytes, all zero) and `m_buf` (0x011e8540, 320 bytes by label spacing, zero) are data units.
- `__Q212RealmService19RealmGatewayAPICore...` (0x01119e28): builds a 144-byte object: generic API base with eight constants (three on the stack), back pointer at +104, vtable at +100, `Encryptor` member at +116, an empty old-style string at +132 copied from `nilRep`, then `replace(0, npos, name, strlen(name))` and the 16-bit value at +136. The 16-bit argument is sign-extended from the low halfword by the routine itself.
- `vsnprintf16__8VIStringPUsiPCcPv` (0x01019f80): widens the narrow format into a stack block of `2 * (2 * strlen + 2)` bytes rounded to 16 with `str8to16`, then calls the wide `vsnprintf16__8VIStringPUsiPCUsPv` (0x0101a7e0), which forwards to `VI_vsnprintf<unsigned short>`.
- `VI_vsnprintf__H1Zc_PX01iPCX01Pv_v` (0x011f52e0) and `VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v` (0x011f59d0): the project's printf. Arguments sit in 8-byte slots. The specification is scanned up to one of `cCdiIouxXeEfgGnpsSh%` (63-byte limit); number formats go to `sprintf`; `I` uses `%lli`; `h` is a hex dump (count slot after the pointer slot) with a space after every fourth byte; `n` falls through into `p`. In the narrow version every 16-bit unit of an `S` argument above 127 becomes `?` (the stored signed byte is compared with the unsigned unit). The wide version halves the size with an unsigned shift and widens bytes 0x80-0x9f through the 1252 table.

## Inferred

- Parameter names of the network functions (`capacity`, `address`, `flags`...) are roles guessed from the single call site (the start-up thread), not SDK names.
- `sMessagePool`, `sFreeMessages`, `sUsedMessages` are a pool of sixteen message nodes and two lists.
- The `S`-conversion quirk and the dead capacity check inside `c` and `%` are copied as observed; they look like compiler-visible source structure, not intent.

## Imports closed (names)

Close__11VIUDPSocket, VIInitUSocketExApi__Fv, VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v, VI_vsnprintf__H1Zc_PX01iPCX01Pv_v,
__Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI, ezNetCnfGetProperties, ezNetCtlSetCombination,
ezNetCtlUpInterface, hostname, sceInetAddress2Name, sceInetAddress2String, sceLibnetRegisterHandler, sceLibnetWaitGetAddress,
vsnprintf16__8VIStringPUsiPCcPv (fourteen authored here).
Gsm_LPC_Analysis, Gsm_Long_Term_Predictor, Gsm_RPE_Encoding, inflateInit2_, inflate_blocks, inflate_blocks_free, inflate_blocks_reset
(authored in `w15-libs-eh-a`; closed once the plan entries below are merged).
Also authored and closed here: `StrcpyS`, `vsnprintf16__8VIStringPUsiPCUsPv`, `m_buf` (the last is data).

## Imports opened

From my units: `StrcpyS` (now authored), `WaitGetAddressAutoUpIf`, `WaitGetAddressNoAutoUpIf`, `ll_init__FP5llist`, `ll_add_tail__FP5llistP5lnode`,
`sFreeMessages`, `sUsedMessages`, `__Q210GenericAPI14GenericAPICorePCcsUiUiUiUiUiUiUiUi`, `__9Encryptor`,
`replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi`, `_vt$Q212RealmService19RealmGatewayAPICore`,
`_VIAssert`, `_VIAssertf`, `__VIAssertf_file`, `__VIAssertf_line`.
From the seven unintegrated `w15-libs-eh-a` units once merged (their own imports, not mine): `APCM_quantization`, `Autocorrelation`,
`Calculation_of_the_LTP_parameters`, `Cut_Calculation_of_the_LTP_parameters`, `Long_term_analysis_filtering`, `Quantization_and_coding`,
`RPE_grid_selection`, `Reflection_coefficients`, `adler32`, `inflate_blocks_new`, `inflate_codes`, `inflate_codes_free`, `inflate_codes_new`,
`inflate_flush`, `inflate_mask`, `inflate_trees_bits`, `inflate_trees_dynamic`, `inflate_trees_fixed`, `zcalloc`, `zcfree`.
The private build with every entry below went from 841 to 855 unresolved names (21 closed, 35 opened, StrcpyS now authored: 34).

## Storage notes (duplicates the integrator should know about)

- `m_buf` is published global by this package; `vi-launcher-eznet-down` still keeps its own file-local `m_buf` under the same asm name, so `ezNetCtlDownInterface` and my two eznetctl functions use different buffers until that unit switches to the global.
- `m_cd` and `m_rpcRunning` (0x01192c6c) are used as externs from `w14-startup-sys-eznet-ctl-client` and `vi-launcher-eznet-ctl`. `ezNetCnfGetProperties` keeps file-local copies of the cnf client state (as `w9-startup-crt-d-init-ez-net-cnf` does), so it will not see the running flag `initEzNetCnf` sets until those units share one definition.
- `sMessagePool` is still an extern (defined nowhere); `sFreeMessages` and `sUsedMessages` too.

## Build

`python3 docs/agents/w16-lib-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` prints the plan entries (below) and can write a private copy of
plan and snapshot; I built with it (`--private-root` outside git, 7,350 sources, exit 0, no retry): 5,855 functions, 855 unresolved names.

## Build-plan entries needed

`profile` is `authored_cpp`, `kind` is `independently_authored_source`. Add each line to `inputs` (next free `input_order`), add each source and header to the
plan `files` map, and bump `default_authored_inputs` by 24. The first 17 lines are this package's units; the last seven are the `w15-libs-eh-a` units that
define the seven names above.

```
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "ezNetCnfGetProperties"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_eznet_cnf_get_properties.o", "profile": "authored_cpp", "sha256": "30156ca03e8182761364c85130c054718fa0de301ed0a458bc300f28b8fcae26", "source": "sources/w16-lib-a-eznet-cnf-get-properties/get_properties.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_eznet_ctl_buf.o", "profile": "authored_cpp", "sha256": "0d8e552b8e62f79ae2e4d93483376e9b867b6cac497a2616bf9ecb8bc6e4f471", "source": "sources/w16-lib-a-eznet-ctl-buf/m_buf.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "ezNetCtlSetCombination"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_eznet_ctl_set_combination.o", "profile": "authored_cpp", "sha256": "ce42b44d983a2e4370235bab62071e91c8531c8defa177d156641a70e095028d", "source": "sources/w16-lib-a-eznet-ctl-set-combination/set_combination.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "ezNetCtlUpInterface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_eznet_ctl_up_interface.o", "profile": "authored_cpp", "sha256": "c0af2e95d0102edf424d1b8e5cd504c1479b40db9449f2f750a19775599e6d52", "source": "sources/w16-lib-a-eznet-ctl-up-interface/up_interface.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_hostname.o", "profile": "authored_cpp", "sha256": "7dce793c7657d1e3192cf725284d71abf8d1cc5ede04f244ca0954e1f797a035", "source": "sources/w16-lib-a-hostname/hostname.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "__Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_realm_gateway_api_core_ctor.o", "profile": "authored_cpp", "sha256": "a4818865f6619bf091b0f0a4e9a9917b1c0a90cd84afbe70c61faceaa857dc99", "source": "sources/w16-lib-a-realm-gateway-api-core-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 208, "name": "sceInetAddress2Name"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_sce_inet_address2name.o", "profile": "authored_cpp", "sha256": "e42b0457129780c0403f000e1759bd389b2a57f5652b1f71467c751ddbac27ff", "source": "sources/w16-lib-a-sce-inet-address2name/address2name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "sceInetAddress2String"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_sce_inet_address2string.o", "profile": "authored_cpp", "sha256": "93b6dd5dd07ab5b591a9282e796e3a11fbd3c1aad72f3589213dd30364eca259", "source": "sources/w16-lib-a-sce-inet-address2string/address2string.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "sceLibnetRegisterHandler"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_sce_libnet_register_handler.o", "profile": "authored_cpp", "sha256": "044948cc8674ff46f4001b301d59fbbedf93a8d2c430a50a99a8c92619f08f27", "source": "sources/w16-lib-a-sce-libnet-register-handler/register_handler.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "sceLibnetWaitGetAddress"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_sce_libnet_wait_get_address.o", "profile": "authored_cpp", "sha256": "e352a1f249d9f50a835d7124a5772f7cfbfb62e4d39968996acfab8285d3dcbe", "source": "sources/w16-lib-a-sce-libnet-wait-get-address/wait_get_address.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "StrcpyS"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_sce_strcpy_s.o", "profile": "authored_cpp", "sha256": "975fc7d78c5233c35843019b65040c5e312ec862b7e1d159c26736dadee4eb08", "source": "sources/w16-lib-a-sce-strcpy-s/strcpy_s.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "Close__11VIUDPSocket"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_udp_socket_close.o", "profile": "authored_cpp", "sha256": "e1b91a068d1fc5376ee1e9845a4ea6b595e62606d2fd6604d43a155d1ec89e08", "source": "sources/w16-lib-a-udp-socket-close/close.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "VIInitUSocketExApi__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_usocket_ex_api_init.o", "profile": "authored_cpp", "sha256": "ab18f02608347ee4bc869ada22654f7c26885cae94f0cbe0d0b8de3cb27e8494", "source": "sources/w16-lib-a-usocket-ex-api-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1844, "name": "VI_vsnprintf__H1Zc_PX01iPCX01Pv_v"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_vi_vsnprintf_narrow.o", "profile": "authored_cpp", "sha256": "a05291e0d39095983a06cf8a0dbc63a4de78cd6f5c9934918e6c6ccff544ee8f", "source": "sources/w16-lib-a-vi-vsnprintf-narrow/vsnprintf.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2220, "name": "VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_vi_vsnprintf_wide.o", "profile": "authored_cpp", "sha256": "0373e9ac2e049a276bbe53b0dffec78619c42def522a7708aad035660fa4ea86", "source": "sources/w16-lib-a-vi-vsnprintf-wide/vsnprintf.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "vsnprintf16__8VIStringPUsiPCcPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_vistring_vsnprintf16_narrow.o", "profile": "authored_cpp", "sha256": "e7b823e40a9c5ff959b8c7bae1848016f1bb4059f68aed93e1b554b6ee564e0c", "source": "sources/w16-lib-a-vistring-vsnprintf16-narrow/vsnprintf16.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "vsnprintf16__8VIStringPUsiPCUsPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16la_vistring_vsnprintf16_wide.o", "profile": "authored_cpp", "sha256": "a74c33cd1a0deabd66e6e7d6f2a676fdf29c59c7d30bd959fe3a8b9e7db20514", "source": "sources/w16-lib-a-vistring-vsnprintf16-wide/vsnprintf16.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "Gsm_Long_Term_Predictor"}], "header_closure": ["sources/w15-libs-eh-a-gsm-common/gsm.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_gsm_long_term_predictor.o", "profile": "authored_cpp", "sha256": "35e25a0774da033a799576999b79ddc21f9bfa4844576bc8c5989f22dc7f5876", "source": "sources/w15-libs-eh-a-gsm-long-term-predictor/long_term_predictor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "Gsm_LPC_Analysis"}], "header_closure": ["sources/w15-libs-eh-a-gsm-common/gsm.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_gsm_lpc_analysis.o", "profile": "authored_cpp", "sha256": "14b03c15b98d8faf073fdcea87ae31ddb0ac9447bdfbaafaaf759564d7d3593b", "source": "sources/w15-libs-eh-a-gsm-lpc-analysis/lpc_analysis.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 792, "name": "Gsm_RPE_Encoding"}], "header_closure": ["sources/w15-libs-eh-a-gsm-common/gsm.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_gsm_rpe_encoding.o", "profile": "authored_cpp", "sha256": "e8a634ed5841a31c4e3627d9e9204b814392bd5265e38800bbc984c2625145ca", "source": "sources/w15-libs-eh-a-gsm-rpe-encoding/rpe_encoding.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3476, "name": "inflate_blocks"}], "header_closure": ["sources/w15-libs-eh-a-zlib-common/inflate.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_inflate_blocks.o", "profile": "authored_cpp", "sha256": "71667fb0b82cdbab290f144cc9d5d6e10f380b5e0581e8001b539f1f98722a01", "source": "sources/w15-libs-eh-a-inflate-blocks/inflate_blocks.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "inflate_blocks_free"}], "header_closure": ["sources/w15-libs-eh-a-zlib-common/inflate.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_inflate_blocks_free.o", "profile": "authored_cpp", "sha256": "c1ce5cb00451b3a8863ce4a76d0728ded8dd2ef4fc083f0fc82cee7bfb961948", "source": "sources/w15-libs-eh-a-inflate-blocks-free/inflate_blocks_free.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 188, "name": "inflate_blocks_reset"}], "header_closure": ["sources/w15-libs-eh-a-zlib-common/inflate.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_inflate_blocks_reset.o", "profile": "authored_cpp", "sha256": "43a700120d543670af53378cffaf573fa489e972b8dab821b2a16ea47ec43cf0", "source": "sources/w15-libs-eh-a-inflate-blocks-reset/inflate_blocks_reset.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 416, "name": "inflateInit2_"}], "header_closure": ["sources/w15-libs-eh-a-zlib-common/inflate.hpp"], "kind": "independently_authored_source", "object_name": "w15eha_inflate_init2.o", "profile": "authored_cpp", "sha256": "f6423bd29b83c40eb7c06f62c5b1dd3ff1c13ae1bc2599ac5b507c3061f837b5", "source": "sources/w15-libs-eh-a-inflate-init2/inflate_init2.cpp"}
```

## Authored but never executed

Everything in this package. The paired runs executed the authored bodies on a private interpreter against the original's bytes with Python stand-ins for
the callees (`sprintf`, `CallRpc`, `MemcpyD`, `strcpy`, the list functions, the assertion functions), and the host checks executed them on the host. No
function ran on the PlayStation 2, in an emulator of the whole game, or on a real network or text input. In particular the formatter's floating-point
conversions were only compared with the original through a scripted `sprintf`; the real `sprintf`/`vfprintf` chain was not run. The 'default' arm of the
formatter (an unlisted conversion byte) is unreachable and was not executed, and the narrow `S` arm's `?` rule and the wide `S` arm's overlap re-read
branch are matched in structure; the overlap branch cannot differ at run time.

## Unknown

- The bodies of the callees listed under "Imports opened", in particular the two `WaitGetAddress*` helpers and what the server does with function numbers 2, 5, 7, 18 and 30.
- The layout of the 144-byte `RealmGatewayAPICore` beyond the offsets used, and the meaning of its eight generic-base constants.
- The 2084-byte message node and the roles of the three message-pool objects; the extents of `sFreeMessages`/`sUsedMessages`.
- Whether `m_buf`'s true extent is 320 bytes (taken from label spacing) and whether `hostname` has other writers.
- The real argument-block layout builders (compiler varargs save areas) for the formatter; slots are read as 8 bytes each as observed.
