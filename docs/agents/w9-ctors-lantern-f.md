# Ant-lantern constructor view data (f)

Package `w9-ctors-lantern-f`, branch `agents/w9-ctors-lantern-f`. Reference: the private `SLUS_205.65`; no game bytes are committed.

## Evidence
- Observed: every symbol in scope is one 4-byte word in `.lit4` (`0x47fa00`..), all normal finite single-precision values. The bytes in each compiled object's `.rodata` equal the executable's bytes at that address (spot-checked 0x481294 by dump comparison; the authored source is generated from the executable bytes and each host test pins the exact bit pattern).
- Observed: callers in `ctors-ant-lantern-constructors` read them as float arguments or copy the word into object fields (actor offsets 88, 96, 168, 224; prop record offsets 36 and 44; one compared against a random roll).
- Unknown: meaning of each constant; the original placement (definitions here do not sit at the original addresses, and literal pooling order is not reproduced).

## Verification
Each unit compiles with the reviewed `authored_cpp` EE profile and has a host test (`host/`, not a build input) that fails unless the bits equal the observed word. One commit per symbol. Nothing ran on the EE. Unit tests under tools were run once at the end.

## Imports closed
- `norrath_ctors_ant_lantern_view_481270`
- `norrath_ctors_ant_lantern_view_481274`
- `norrath_ctors_ant_lantern_view_481278`
- `norrath_ctors_ant_lantern_view_48127c`
- `norrath_ctors_ant_lantern_view_481280`
- `norrath_ctors_ant_lantern_view_481284`
- `norrath_ctors_ant_lantern_view_481288`
- `norrath_ctors_ant_lantern_view_48128c`
- `norrath_ctors_ant_lantern_view_481290`
- `norrath_ctors_ant_lantern_view_481294`
- `norrath_ctors_ant_lantern_view_481298`
- `norrath_ctors_ant_lantern_view_4812d8`
- `norrath_ctors_ant_lantern_view_4812dc`
- `norrath_ctors_ant_lantern_view_4812e0`
- `norrath_ctors_ant_lantern_view_4812e4`
- `norrath_ctors_ant_lantern_view_4812e8`
- `norrath_ctors_ant_lantern_view_4812ec`
- `norrath_ctors_ant_lantern_view_4813bc`
- `norrath_ctors_ant_lantern_view_4813c0`
- `norrath_ctors_ant_lantern_view_4813c4`
- `norrath_ctors_ant_lantern_view_4813c8`
- `norrath_ctors_ant_lantern_view_4813cc`
- `norrath_ctors_ant_lantern_view_4813d0`
- `norrath_ctors_ant_lantern_view_4813d4`
- `norrath_ctors_ant_lantern_view_4814e4`
- `norrath_ctors_ant_lantern_view_4814e8`
- `norrath_ctors_ant_lantern_view_4814ec`
- `norrath_ctors_ant_lantern_view_4814f0`
- `norrath_ctors_ant_lantern_view_4814f4`
- `norrath_ctors_ant_lantern_view_4814f8`
- `norrath_ctors_ant_lantern_view_481540`
- `norrath_ctors_ant_lantern_view_481544`
- `norrath_ctors_ant_lantern_view_481704`
- `norrath_ctors_ant_lantern_view_481708`
- `norrath_ctors_ant_lantern_view_481890`
- `norrath_ctors_ant_lantern_view_481894`
- `norrath_ctors_ant_lantern_view_481898`
- `norrath_ctors_ant_lantern_view_48189c`
- `norrath_ctors_ant_lantern_view_4818a0`
- `norrath_ctors_ant_lantern_view_4818a4`

## Imports opened
None.

## Build-plan entries needed
Each needs an `inputs` entry: `kind: independently_authored_source`, `profile: authored_cpp`, `header_closure: []`, `object_name: <name>.o`, `source` as below, same sha256 in `files`; `input_order` by the integrator.

| Unit | source | sha256 |
|---|---|---|
| `norrath_ctors_ant_lantern_view_481270` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481270.cpp` | `60060952809e2c6ab488a8fd79109f9cc1c6c13194e2eda8ca7feb0add660250` |
| `norrath_ctors_ant_lantern_view_481274` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481274.cpp` | `a9ce28a6527d0d170f90211798c28e6f689290dbfeeeb1d7e4685cdf7db7c84f` |
| `norrath_ctors_ant_lantern_view_481278` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481278.cpp` | `4d2adb63cd49c3a90ce81c938bb15ab1cbf0c09e52d43eb5f2817553fe93d1d8` |
| `norrath_ctors_ant_lantern_view_48127c` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_48127c.cpp` | `001264144432fda841ffd3f8e4f5232fa4b2bb5603dc1f982a184dcbcaa08914` |
| `norrath_ctors_ant_lantern_view_481280` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481280.cpp` | `7a25e749fda3570c06e663c615f87ca7d109b37a15b828aff15dac53d38462e7` |
| `norrath_ctors_ant_lantern_view_481284` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481284.cpp` | `925aee5f4a194ef3ebddd251785aceb93708af5dc35f38cdac65c547369ff795` |
| `norrath_ctors_ant_lantern_view_481288` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481288.cpp` | `191c679b1d98649bc302000c285d6657991992790f4939085b0ffb6bfcedac05` |
| `norrath_ctors_ant_lantern_view_48128c` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_48128c.cpp` | `91891806cff39ccbfd7a190f05d53a6d967ef233b47b6b1489cbaf4edb61c8fb` |
| `norrath_ctors_ant_lantern_view_481290` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481290.cpp` | `be607dee7bb5862b57f449f22e68c8a477f2afd8c10daa6bca8eca55a385a4d7` |
| `norrath_ctors_ant_lantern_view_481294` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481294.cpp` | `c2b68d1e7fce17384f08f9e7cd200b3da4c7516f70035868bb1d6b803e866e35` |
| `norrath_ctors_ant_lantern_view_481298` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481298.cpp` | `2ce4c6c1b80c06218164fec10adb7777f4cd776ccdfc6ee2d9bfc218ef2b4b4e` |
| `norrath_ctors_ant_lantern_view_4812d8` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812d8.cpp` | `60a99d2aee0e46b5ec0316c8f3901e203ea42ba85fa861f03da381cda0ef2416` |
| `norrath_ctors_ant_lantern_view_4812dc` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812dc.cpp` | `2c1666e2960b19c4e197bac19e90a7aa3c83148b3ee68f1c520bb2280990be9b` |
| `norrath_ctors_ant_lantern_view_4812e0` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812e0.cpp` | `8872bcb1c88caac86fd2be06a54951981de18c9af5f96ecbdd580ccc06222786` |
| `norrath_ctors_ant_lantern_view_4812e4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812e4.cpp` | `1d93b09b2b03b94e16775d94e091df9cfdca116bc3df1f8fc9542102b1e19346` |
| `norrath_ctors_ant_lantern_view_4812e8` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812e8.cpp` | `596e37d10a801c35410cec0636880c6b1bf8035b206fe3e55f159315b895ed7e` |
| `norrath_ctors_ant_lantern_view_4812ec` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4812ec.cpp` | `038d5a4f67ad84f3ac5297db339b71231e02d1b7f6a9d7d61fd23b82613c9399` |
| `norrath_ctors_ant_lantern_view_4813bc` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813bc.cpp` | `047b099550c419fdf257f547e9db8a931df225963cdd6148e75f6182a9fff5b3` |
| `norrath_ctors_ant_lantern_view_4813c0` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813c0.cpp` | `703e5ca1e369b90f03716b1a50435d0e99bf5a5db32d2ecc408f1095e58d1e8a` |
| `norrath_ctors_ant_lantern_view_4813c4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813c4.cpp` | `cf634f5697dae59b94d9d8d0ab5415ee1decefcaf160ba31175ddb64bc1484f9` |
| `norrath_ctors_ant_lantern_view_4813c8` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813c8.cpp` | `0109e324e71421dd6d9cac3178939f95774e10e987ac49854ed52630e0382925` |
| `norrath_ctors_ant_lantern_view_4813cc` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813cc.cpp` | `7552549ca19f2ee5dd1e914c8fd070e8b6e70af42be38529fed30415ba0f4e8a` |
| `norrath_ctors_ant_lantern_view_4813d0` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813d0.cpp` | `88e477e12bd18d249b431d0ad1519611774f2e29fa58a2c496eb1a9820e7e086` |
| `norrath_ctors_ant_lantern_view_4813d4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4813d4.cpp` | `d40d7a47c56ab0cc95e70931559517d6f707506748df0a243e3f5d3106641d85` |
| `norrath_ctors_ant_lantern_view_4814e4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814e4.cpp` | `44a7520b538ec72de2a14f8b2cddaecd1c4393c4749514378169f52c664b8e4f` |
| `norrath_ctors_ant_lantern_view_4814e8` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814e8.cpp` | `bb8fbbb866cd25786a603681cee42fa493fede61f7d4fdb415e6d479554342c3` |
| `norrath_ctors_ant_lantern_view_4814ec` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814ec.cpp` | `fda142d28b1871fc0be860d544517abd7c87175557e18f7e82100da28b251a98` |
| `norrath_ctors_ant_lantern_view_4814f0` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814f0.cpp` | `8b799af310c8e87834dd50281f3474ef0f39ac837889549a4d64b76fef52fee8` |
| `norrath_ctors_ant_lantern_view_4814f4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814f4.cpp` | `2db9f699c96fe4bb291081628c71775cc299a11d4d569872b1e2425413c751b5` |
| `norrath_ctors_ant_lantern_view_4814f8` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4814f8.cpp` | `99b7a6da8822d3b7c40e528aaf2bd1cb22bc5d7d9c285acaefa275931931a088` |
| `norrath_ctors_ant_lantern_view_481540` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481540.cpp` | `4cefebb2a1e033df8fbea3db14ee8bf596d2a0d8b3eb3fae1d4032a13d164772` |
| `norrath_ctors_ant_lantern_view_481544` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481544.cpp` | `7e1310193e00ac7dbb53abf9c50622d5b342e6d22a88bfb63523b310aea242f4` |
| `norrath_ctors_ant_lantern_view_481704` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481704.cpp` | `23b9fbb8ba48a7c23606634201d9763d1979be6b4b2357cbd100fd300010c88b` |
| `norrath_ctors_ant_lantern_view_481708` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481708.cpp` | `768fe036fd5f749c5fc5b239f3edf1001476bc9c9c9cea769c97a463afac088e` |
| `norrath_ctors_ant_lantern_view_481890` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481890.cpp` | `60f29bd0b745873a44b6be1a75f86a5728900f55ef0a0748e8622dc3727dcc3a` |
| `norrath_ctors_ant_lantern_view_481894` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481894.cpp` | `bf25bd186b107092e33919b3c29de0c30535a1efbfa94ad9d677bf8bd8a1aa72` |
| `norrath_ctors_ant_lantern_view_481898` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_481898.cpp` | `bf8856e3d836d172bebed28dbb3a6adeee240faadb5cec33c6964ee4a92d83ab` |
| `norrath_ctors_ant_lantern_view_48189c` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_48189c.cpp` | `58a1b980d7ddb13805765c9c3855b9839233a56b6d1cf6464733399030db70d7` |
| `norrath_ctors_ant_lantern_view_4818a0` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4818a0.cpp` | `f16821769437340249157d14908da9d372f61c064d467dd73b86eed4e1012299` |
| `norrath_ctors_ant_lantern_view_4818a4` | `sources/w9-ctors-lantern-f-views/norrath_ctors_ant_lantern_view_4818a4.cpp` | `3fdebf3479c0d08d987f05b8a82c3fe07aedd75fa028b835481f993464c85c4e` |

## Authored but never executed
All of them: compiled and host-compared only; no EE or gameplay execution.

## Unknown
Semantic names and purposes of the constants; final `.lit4` layout in a linked image.
