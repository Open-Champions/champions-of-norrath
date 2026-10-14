#!/bin/sh
# usage: mutq.sh test-arg-script unit-dir file 'perl-expr'  -> runs test on a mutated copy, restores
S=$1; W=$2; F=/Users/case/norrath-agents/wt/w16-tmpl-list-a/experimental/sources/$3; E=$4
B=/tmp/tla_bak_$$.cpp; cp "$F" $B
perl -pi -e "$E" "$F"
cmp -s "$F" $B && echo "MUTATION DID NOT APPLY"
O=$(timeout 100 python3 $S $W 2>&1 | grep -E "HALT|scenarios|rror" | tail -2); [ -z "$O" ] && O="NO-OUTPUT (timeout or crash: mutant caught)"; echo "$O"
cp $B "$F"
