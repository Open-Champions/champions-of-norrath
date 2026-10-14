# ant-lantern constructor and view data (d)

Package `w9-ctors-lantern-d`, branch `agents/w9-ctors-lantern-d`. Forty data objects read by the
`ctors-ant-lantern-constructors` units: six C strings (`.rodata`) and thirty-four `.lit4` words.
Source is under `experimental/sources/w9-ctors-lantern-d-views/`, one object per file and per commit.

## Evidence

- Observed: private executable `work/private/SLUS_205.65`; section placement `.rodata` 0x454300,
  `.lit4` 0x47fa00. Bytes at each address were read from the file by a script kept outside git.
- Observed: strings are the characters plus NUL; zero padding after is not part of the object.
  Each `.lit4` word is a 4-byte object; neighbours are separate words, so overlapping reads in the
  file (each address followed by the next literal) are not part of an object.
- Checks per object: compiled with the reviewed `authored_cpp` EE profile; the compiled object's
  `.rodata` was searched for the executable's exact bytes; a host test (`host/`, not an EE input)
  compares the bytes. All 40 passed. Nothing ran on the EE.
- Inferred: the `.lit4` words are single floats (callers load them with float reads or copy the raw word
  into a field). The strings' roles are as the callers use them: `476b90` one of two texts chosen in the
  jump gate constructor; `4770d0`/`4770e8` compared case-insensitively with the female dark elf's name;
  `477798`/`4777d8` lump-resource arguments and `4777c8` a tag name in the catapult constructor.
- Unknown: the meaning (gameplay role) of each float; original placement addresses (definitions do not
  place objects at their original addresses); whether `.lit4` merging would alter link layout.

## Imports closed

- `norrath_ctors_ant_lantern_view_476b90`
- `norrath_ctors_ant_lantern_view_4770d0`
- `norrath_ctors_ant_lantern_view_4770e8`
- `norrath_ctors_ant_lantern_view_477798`
- `norrath_ctors_ant_lantern_view_4777c8`
- `norrath_ctors_ant_lantern_view_4777d8`
- `norrath_ctors_ant_lantern_view_480b10`
- `norrath_ctors_ant_lantern_view_480b14`
- `norrath_ctors_ant_lantern_view_480b20`
- `norrath_ctors_ant_lantern_view_480b24`
- `norrath_ctors_ant_lantern_view_480b28`
- `norrath_ctors_ant_lantern_view_480b2c`
- `norrath_ctors_ant_lantern_view_480b30`
- `norrath_ctors_ant_lantern_view_480b34`
- `norrath_ctors_ant_lantern_view_480b38`
- `norrath_ctors_ant_lantern_view_480b3c`
- `norrath_ctors_ant_lantern_view_480b40`
- `norrath_ctors_ant_lantern_view_480b44`
- `norrath_ctors_ant_lantern_view_480b48`
- `norrath_ctors_ant_lantern_view_480b4c`
- `norrath_ctors_ant_lantern_view_480ba8`
- `norrath_ctors_ant_lantern_view_480bac`
- `norrath_ctors_ant_lantern_view_480bb0`
- `norrath_ctors_ant_lantern_view_480bb4`
- `norrath_ctors_ant_lantern_view_480cc8`
- `norrath_ctors_ant_lantern_view_480ccc`
- `norrath_ctors_ant_lantern_view_480cd0`
- `norrath_ctors_ant_lantern_view_480cd4`
- `norrath_ctors_ant_lantern_view_480cd8`
- `norrath_ctors_ant_lantern_view_480cdc`
- `norrath_ctors_ant_lantern_view_480ce0`
- `norrath_ctors_ant_lantern_view_480ce4`
- `norrath_ctors_ant_lantern_view_480ce8`
- `norrath_ctors_ant_lantern_view_480cec`
- `norrath_ctors_ant_lantern_view_480d28`
- `norrath_ctors_ant_lantern_view_480e58`
- `norrath_ctors_ant_lantern_view_480e5c`
- `norrath_ctors_ant_lantern_view_480e60`
- `norrath_ctors_ant_lantern_view_480e64`
- `norrath_ctors_ant_lantern_view_480e68`

## Imports opened

None.

## Build-plan entries needed

Not added to `experimental/build-plan.json` (shared). Each needs an `inputs` entry with
`kind: independently_authored_source`, `profile: authored_cpp`, `header_closure: []`,
`object_name: <name>.o`, `source: sources/w9-ctors-lantern-d-views/<name>.cpp`, the sha256 below
(also in `files`), and an `input_order` assigned by the integrator. Each object has no function body.

| Unit | sha256 |
|---|---|
| `norrath_ctors_ant_lantern_view_476b90` | `90dd8a6f3ac298e16604bb7bc181cb337a4c8df66cbd431ea51ce832085e03c1` |
| `norrath_ctors_ant_lantern_view_4770d0` | `a101ad89fd8b4c5f1c01e84c22662dc3299183017f29d4dbbaa8307fc43e7e52` |
| `norrath_ctors_ant_lantern_view_4770e8` | `fc60bd6860c019ffa4c96acb34733f552644833792428ad0d6385a3eed5205d8` |
| `norrath_ctors_ant_lantern_view_477798` | `eb01d85b02affa321724dabf8f96599983f86d6d58789257932d33b7ea11ad7b` |
| `norrath_ctors_ant_lantern_view_4777c8` | `fcaebcd305244a7400d5b1528637f63961af631bdd8da106e94114c75642a28c` |
| `norrath_ctors_ant_lantern_view_4777d8` | `f52a7e8bca4da8684fbae26f34cdd3b8f95942cb80fe3c783604e9890e6b8eb8` |
| `norrath_ctors_ant_lantern_view_480b10` | `61b77dc0e30088b39c46975e820afa995b9dcc4021dbe51629b817b2fcee5790` |
| `norrath_ctors_ant_lantern_view_480b14` | `4c3e5f7a807a96de9ef250b9d3fd410cfcecf258e78baf14787b0a50cdc06eda` |
| `norrath_ctors_ant_lantern_view_480b20` | `ccd019f3b40ea57bfaad8d562ca06ed23d05b1c476f603d1f74a24560f7c4048` |
| `norrath_ctors_ant_lantern_view_480b24` | `4a989afe6f932847142954e1e4842df6723fe13afe8f40c59184eb30c2f64f67` |
| `norrath_ctors_ant_lantern_view_480b28` | `9b6ab133ff4edc29808270df8b9c00da876de7aea756af59811a0d952e18491e` |
| `norrath_ctors_ant_lantern_view_480b2c` | `50dae8e80009c6f9402891ecefc88d415e1ff99ee2397452dd8b26821c080bc1` |
| `norrath_ctors_ant_lantern_view_480b30` | `7bf3f00987d17d3aa887e14274f1ed6e839099495faccc0e81ecdefa517dcf2a` |
| `norrath_ctors_ant_lantern_view_480b34` | `c7b2378f1fd8fd25c018072c923d725a50d4e31c4838463c41a9974fa92d0db9` |
| `norrath_ctors_ant_lantern_view_480b38` | `2c87037d469caf6e2f5869dd0e5dff89f74c47ffa3ee2450e39534332e11bde9` |
| `norrath_ctors_ant_lantern_view_480b3c` | `4edd8beed07447c10e6953ad187fa02a6f8f4da3375de04d04afa69221dac127` |
| `norrath_ctors_ant_lantern_view_480b40` | `9c1249803328d1746cd9bb8015c3735475cd03855955ba55acd692fa16e69187` |
| `norrath_ctors_ant_lantern_view_480b44` | `a7c2b896507d1c396539340a0cf4322137b99ccab7800e948f5a7d944f5cf3eb` |
| `norrath_ctors_ant_lantern_view_480b48` | `e8604430d5b709c9bf5671a78772483e08c89f3b30b82bdd311ab8e033e29b98` |
| `norrath_ctors_ant_lantern_view_480b4c` | `dd5b1d7a3ad8186e14521c1b3324ba6b847d8db25f646edd47d22dce75ae9798` |
| `norrath_ctors_ant_lantern_view_480ba8` | `4fe12d9be18134b2cfa6cbd322d1b8168464b6f5a75b9fd0fe877fdbd9f2568b` |
| `norrath_ctors_ant_lantern_view_480bac` | `2c04ad12875a0552562c51b19bdf87206ccbf89cd3eb01fe8f75c4fad2cd7e46` |
| `norrath_ctors_ant_lantern_view_480bb0` | `f121f32590c94001b574ff6366d7f980739890693b33b054d6b3442abbe32b2d` |
| `norrath_ctors_ant_lantern_view_480bb4` | `454d5c2e5bb665852cca1f0857a3780c72b69c1d769ac6ebfaa2ef163f93e25c` |
| `norrath_ctors_ant_lantern_view_480cc8` | `bab3c55271fcc0f970bc401d762f686d1c5ae59a4f796abaad5dc7ed391fa213` |
| `norrath_ctors_ant_lantern_view_480ccc` | `ee23d250b5d470af3244eedb54b67e970b9aa3432b7dfcb87a6ebae7df06fa9c` |
| `norrath_ctors_ant_lantern_view_480cd0` | `b07266b12c2021e49e542b2c0a7dfaeb4aafe9cf8bc56e905faf9a3145b3be79` |
| `norrath_ctors_ant_lantern_view_480cd4` | `36a73be8177c6b8e5ebd9d3acd47c39ad844cb21271a8fcd0417c4d7ed220287` |
| `norrath_ctors_ant_lantern_view_480cd8` | `ac6856b0e1c087b9785248a350735b7e627a74c68b8d3f8e0e5159843cf18645` |
| `norrath_ctors_ant_lantern_view_480cdc` | `421d4b3b26598d5e6e1068d00393c6a93639d01c63f5b7b7614e20d1414f4740` |
| `norrath_ctors_ant_lantern_view_480ce0` | `2c7f89159da88274048bfe39bdbe27e0a175e398092a0c09e5477fa4a50b23be` |
| `norrath_ctors_ant_lantern_view_480ce4` | `73cf0da5fa3b328c1148a8818fbcff24c6fcbc3342e2f8dd7bc6b2753be29af7` |
| `norrath_ctors_ant_lantern_view_480ce8` | `3a99580bebd25ddee9729ea01a1e318a3dfe2538333d5294cc0f55b684803aba` |
| `norrath_ctors_ant_lantern_view_480cec` | `0fbfd5e58e7b9f06c7ba473e289fa8f217b18fe85e7cd51db0bdc04316141300` |
| `norrath_ctors_ant_lantern_view_480d28` | `078afe02682b66f4bd3a4850bb74edb3c359267fe51f070c8af9823c4c708594` |
| `norrath_ctors_ant_lantern_view_480e58` | `239f790a1b2aea22c6f597ede07cfd5727ba5485c8b76456a493e89ce5d42445` |
| `norrath_ctors_ant_lantern_view_480e5c` | `b3b7672a2eaef780c73afa69933bc03ffb9b718d672fca19b5e2ccbae9d5dc53` |
| `norrath_ctors_ant_lantern_view_480e60` | `2cfa5a717e793a0d36e744d4b6f1dd3ae43abb7bb9f780f95c84ce9a735d76fb` |
| `norrath_ctors_ant_lantern_view_480e64` | `987e88b57706c104354f6a570047ebbe8e794ec5770257f1d4791241a8a2cffb` |
| `norrath_ctors_ant_lantern_view_480e68` | `faf12916445560cdf2180327c05c29d297ed23095565209d935101609162be6d` |

## Authored but never executed

Everything. These are data objects; only the compile and host byte comparisons above were run. No partial
build or link was run on this branch (so the current unresolved-import list was taken from the task text,
not regenerated).

## Unknown

The gameplay meaning of the float constants; final link layout; whether the original `.lit4` literal pool
shares or orders words in any way a separate-object definition would change.
