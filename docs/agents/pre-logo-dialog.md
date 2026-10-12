# pre-logo-dialog

Takeover uses the existing `agents/pre-logo-dialog` worktree. It was clean at
`00dcc88`; no uncommitted work needed retaining or discarding. The predecessor
had published fourteen package commits but no handoff note. All fourteen source
units freshly compiled with the plan's reviewed EE `authored_cpp` flags; all
fourteen host contracts passed with ASan/UBSan. The private executable SHA-256
was checked against `targets/SLUS-20565/baseline.json`.

Observed evidence is from that pinned image; addresses below are provenance,
not absolute-address bindings in the authored link. Inferred C++ interfaces
cover accessed fields only. Host contracts test call arguments and state changes;
they are not original-machine execution or exact-byte comparisons.

## Takeover additions

`helpMessageFrame__Fv` at `0x001c4b18`, 1376 original bytes: decrements a positive
control pause before deciding the global/per-player help path. The global path
pauses offline play and skips drawing below 3000 display-list slots, but still
checks dismissal when the control pause is zero. Every matching controller is
processed, even after an earlier dismissal. The multiplayer path requires a
release before a new press and does not use the global drawing-space gate.
Host tests exercise countdown, alpha, draw coordinates, low-space dismissal,
multiple controllers, signed player-byte forwarding, and release/press behavior.
Fresh reviewed EE compilation and ASan/UBSan host execution passed.

## Working environment

`git fetch origin` failed on shared malformed ref
`refs/heads/agents/iop-module-boot 2`. That unrelated shared ref was not changed.
Private references, build plans, compiled objects, and logs stay outside git.

## Imports closed

Pending final private link measurement; inherited units and additions define
package symbols but are not yet selected in the shared build plan.

## Imports opened

Pending final object/import inventory. All external helpers remain imports.

## Build-plan entries needed

Pending final measured entries. Do not select host checks. Shared plan, snapshot,
receipts, tools, and other packages are untouched.

## Authored but never executed

All package EE implementations. Host contracts ran; no EE execution, startup,
rendered dialog, pause menu, or gameplay has been demonstrated.

## Unknown

Camera temporary extent and player storage integration remain under review.
The remaining dialog and pause routines are still being reconstructed.
