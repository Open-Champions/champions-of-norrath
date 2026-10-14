# ant-lantern constructor and view data (h)

Package `w9-ctors-lantern-h`, branch `agents/w9-ctors-lantern-h`. Sources are in `experimental/sources/w9-ctors-lantern-h-views/`, one file per symbol, each its own commit. Host checks are in `host/` and are not EE inputs.

Private reference: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that file's symbol values. No game bytes beyond the short tag texts and the name table, and no disassembly, are committed.

## What the views are

- Observed: 0x485420 to 0x4860f8 lie in `.sdata` (0x483200 to 0x4867de). The 33 string views are NUL-terminated C strings in eight-byte slots; the padding after the NUL is not part of the view.
- Observed: `norrath_ctors_ant_lantern_view_485580` is a run of eight-byte name slots. The one caller indexes 0..10, so the view is `char[11][8]` (88 bytes). Inferred extent; the image has more slots after the eleventh (not defined here).
- Observed: `..._486030` is ELF symbol `jumping`, size 4, initial bytes zero, in `.sdata`. `..._486da4` is ELF symbol `jumpGateHurtDest`, recorded size 0, in `.sbss`; the next symbol is 4 bytes later. Each caller loads one word, so both are 4-byte zero words. Both are emitted in `.bss` by the compiler; the placement and the section identity are not claimed.
- Unknown: what the two words mean, and the extent of the name table past index 10.
- Names stay the address-based import names. The literal texts are committed because the callers compare or pass them; the tag/mode roles are visible in the constructors but no names are proposed.

## Checks

Each file compiled with the reviewed EE g++ using the `authored_cpp` flags from `experimental/build-plan.json`. `nm -S` on each object showed the symbol with the expected size. Each host check (c++ on this Mac) returns non-zero if the bytes, size, or NUL padding differ. The private image bytes were read to author the values; the checks compare the sources with those values written out in the check.

## Imports closed

- `norrath_ctors_ant_lantern_view_485420`
- `norrath_ctors_ant_lantern_view_485438`
- `norrath_ctors_ant_lantern_view_485440`
- `norrath_ctors_ant_lantern_view_485480`
- `norrath_ctors_ant_lantern_view_485488`
- `norrath_ctors_ant_lantern_view_4854c8`
- `norrath_ctors_ant_lantern_view_4854d0`
- `norrath_ctors_ant_lantern_view_4854d8`
- `norrath_ctors_ant_lantern_view_4854e0`
- `norrath_ctors_ant_lantern_view_4854e8`
- `norrath_ctors_ant_lantern_view_4854f0`
- `norrath_ctors_ant_lantern_view_4854f8`
- `norrath_ctors_ant_lantern_view_485500`
- `norrath_ctors_ant_lantern_view_485530`
- `norrath_ctors_ant_lantern_view_485538`
- `norrath_ctors_ant_lantern_view_485540`
- `norrath_ctors_ant_lantern_view_485548`
- `norrath_ctors_ant_lantern_view_485560`
- `norrath_ctors_ant_lantern_view_485568`
- `norrath_ctors_ant_lantern_view_485570`
- `norrath_ctors_ant_lantern_view_485578`
- `norrath_ctors_ant_lantern_view_485580`
- `norrath_ctors_ant_lantern_view_485760`
- `norrath_ctors_ant_lantern_view_485768`
- `norrath_ctors_ant_lantern_view_486030`
- `norrath_ctors_ant_lantern_view_486038`
- `norrath_ctors_ant_lantern_view_486040`
- `norrath_ctors_ant_lantern_view_486048`
- `norrath_ctors_ant_lantern_view_486050`
- `norrath_ctors_ant_lantern_view_486058`
- `norrath_ctors_ant_lantern_view_486060`
- `norrath_ctors_ant_lantern_view_4860e0`
- `norrath_ctors_ant_lantern_view_4860e8`
- `norrath_ctors_ant_lantern_view_4860f0`
- `norrath_ctors_ant_lantern_view_4860f8`
- `norrath_ctors_ant_lantern_view_486da4`

## Imports opened

None.

## Build-plan entries needed

Append with fresh `input_order` values; `default_authored_inputs` increases by 36. Do not add `host/`.

```json
{"source":"sources/w9-ctors-lantern-h-views/view_485420.cpp","object_name":"norrath_ctors_ant_lantern_view_485420.o","sha256":"c65001ab4e877b60ca550da11d791650002abd0697e0aa09a3114a6f15eee596","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485438.cpp","object_name":"norrath_ctors_ant_lantern_view_485438.o","sha256":"1104c2fcc898f76cddcb7006080dca8b7c412941c574f22a69d776e6291ff88e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485440.cpp","object_name":"norrath_ctors_ant_lantern_view_485440.o","sha256":"c1a99f383cf565675ab95e44390a40c7cd31c9d14dba6ad7392a4f0fa00d8302","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485480.cpp","object_name":"norrath_ctors_ant_lantern_view_485480.o","sha256":"028848f984f2fce76470a9cf1215f64d07920df3886d78eb0823add128368a8d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485488.cpp","object_name":"norrath_ctors_ant_lantern_view_485488.o","sha256":"e0210a5dbf8d74a8ef31cafcd24c5e720ba185c9944524e17f9a23990d93e439","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854c8.cpp","object_name":"norrath_ctors_ant_lantern_view_4854c8.o","sha256":"37dd02a4b9eb31202577fb3b3b6ac762969d419fd12231a8bed5feb08289bdbb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854d0.cpp","object_name":"norrath_ctors_ant_lantern_view_4854d0.o","sha256":"1ec627daaa642e30c1ae18db411375a22ef3215d3d02338b8f9c7c7fa0d3608e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854d8.cpp","object_name":"norrath_ctors_ant_lantern_view_4854d8.o","sha256":"f6580b5434eb5bdd679a8ffbfcdc1545825a48d3a97d865ce765d1086adec00d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854e0.cpp","object_name":"norrath_ctors_ant_lantern_view_4854e0.o","sha256":"1a8e3f26cf5a190d6ae3f4b8222c75167700e01dc7a39627b82254c3b4432126","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854e8.cpp","object_name":"norrath_ctors_ant_lantern_view_4854e8.o","sha256":"edb6b65f7e0e62500f2c671428ba40c1e42309875fcd83a3e6a91cce07d5042e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854f0.cpp","object_name":"norrath_ctors_ant_lantern_view_4854f0.o","sha256":"763f390409f82251ddef78bde0190a45199cd49aa4a0652fb99553aea0b9643b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4854f8.cpp","object_name":"norrath_ctors_ant_lantern_view_4854f8.o","sha256":"0ef9a2a03b1e9d63e95426a040e311b67534c3307de5677fb9c27425bf3f05be","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485500.cpp","object_name":"norrath_ctors_ant_lantern_view_485500.o","sha256":"4e5fcae2555a0eda6b948424e0acf02dde6bc9cce4f55d634b71dd5dbb0cd454","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485530.cpp","object_name":"norrath_ctors_ant_lantern_view_485530.o","sha256":"eebd7bde93f9505baf364fd127d37d2e84a96719f72245a285bf77a3d504e496","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485538.cpp","object_name":"norrath_ctors_ant_lantern_view_485538.o","sha256":"60d88038aa1bf532d00a454fc08a28b7a161024fedff0c1a379fb42922afe2f2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485540.cpp","object_name":"norrath_ctors_ant_lantern_view_485540.o","sha256":"3742e8ee689a997feae35065846e11d95271269e6610bd4227026f80cfde345f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485548.cpp","object_name":"norrath_ctors_ant_lantern_view_485548.o","sha256":"a1af123f15acd02a8dd666c44dffd31722fb94e99c386ba74f02511baa0e4572","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485560.cpp","object_name":"norrath_ctors_ant_lantern_view_485560.o","sha256":"f75e506dd51c1c94f16e84a148fd7748f7ad8e3d63ac3772231a54abfee6a623","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485568.cpp","object_name":"norrath_ctors_ant_lantern_view_485568.o","sha256":"347f75e2dccce0558fd44308c74314bb5385358ade806a96aa9564873d1908b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485570.cpp","object_name":"norrath_ctors_ant_lantern_view_485570.o","sha256":"af646e1c955b9ca4fd9dff506fbd9bb687d4d378c23688a46cfd057753e1bdf6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485578.cpp","object_name":"norrath_ctors_ant_lantern_view_485578.o","sha256":"b4b0d143c9ed6d21fed0951fbbb9ddb358444d2a257e28fe4f4eff5216f8d0b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485580.cpp","object_name":"norrath_ctors_ant_lantern_view_485580.o","sha256":"ea45a28190bb1b784d60dac280ad0ea146efe22be996adf0a3e1dbddbb7e4043","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485760.cpp","object_name":"norrath_ctors_ant_lantern_view_485760.o","sha256":"5e29a9992328a0b55d15da083b5b9c76bc7bd4b2303a93ffc95be36cc433393b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_485768.cpp","object_name":"norrath_ctors_ant_lantern_view_485768.o","sha256":"c9bd1ce50dfec989a447d353a7734ef3eacd1b6a7ce0aec5675fa0513519d633","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486030.cpp","object_name":"norrath_ctors_ant_lantern_view_486030.o","sha256":"34105a7e371b4629281d5c17f68eec3866ec1f48cb94f72ae4a39c6c4a1317ea","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486038.cpp","object_name":"norrath_ctors_ant_lantern_view_486038.o","sha256":"cbe84e4b9cf5ac7b85a00dc191fb98b3f6b9d1116482fa9aa406b29057593fbb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486040.cpp","object_name":"norrath_ctors_ant_lantern_view_486040.o","sha256":"4823c71c906afa1a9b5934380875737f9193702ddad4e2629a662e10d17add6b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486048.cpp","object_name":"norrath_ctors_ant_lantern_view_486048.o","sha256":"012702ea1b521ae5dfb133a76f04471c12cb316fd7b7021f637c3eaf61464db3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486050.cpp","object_name":"norrath_ctors_ant_lantern_view_486050.o","sha256":"c77087f2f673a2dfd48922c0deaa58e900f2ffe137788b69469a3870378f2db1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486058.cpp","object_name":"norrath_ctors_ant_lantern_view_486058.o","sha256":"29fed935fccf8258cb71f14276ef554c9d2670316816dc1f83a95c552795132c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486060.cpp","object_name":"norrath_ctors_ant_lantern_view_486060.o","sha256":"20f617ac44527ef5b48740b7be50b67ae8f6c470cfa355d0299c981537bfaefb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4860e0.cpp","object_name":"norrath_ctors_ant_lantern_view_4860e0.o","sha256":"f0bac5a87fda2a20aa4bc14b00c67fc96bd737905ab94142085c1f9cb7cdae36","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4860e8.cpp","object_name":"norrath_ctors_ant_lantern_view_4860e8.o","sha256":"76413329101b3f8876bd4b3629d3069f8cfa8cc00ff9bf707e625695033e5b54","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4860f0.cpp","object_name":"norrath_ctors_ant_lantern_view_4860f0.o","sha256":"0ee582905b39518f0ba862acda003756399852e7d69a017e27f61e0b19db560d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_4860f8.cpp","object_name":"norrath_ctors_ant_lantern_view_4860f8.o","sha256":"37e863de979d54e23910b4e4f291deb092678fcc4aa67186c3ade680e87b8861","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-h-views/view_486da4.cpp","object_name":"norrath_ctors_ant_lantern_view_486da4.o","sha256":"80958ba8a64152b6431990437e42a8db1db31afd0149a2c4851d4428f211162f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
```

## Authored but never executed

All 36 objects. Nothing ran on the EE or in gameplay. The constructors that read them were not re-run against these definitions here; no partial build was run (single units only).

## Unknown

- Meaning of the words at 0x486030 and 0x486da4, and why 0x486da4 has recorded size 0.
- Name-table extent after index 10, and whether the slots after it belong to another view.
- These definitions close imports; they do not place objects at the original addresses or claim `.sdata`/`.sbss` identity. Texts are duplicated where the image has the same text at several addresses (for example "gold", "bomb").
