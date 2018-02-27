# um-assignment

Universal Machine assignment.

* Read um-spec.txt and implement the interpreter. 
* Run `sandmark.umz`; consult `sandmark.orig` reference output to ensure the minimal correctness.
* Run `codex.umz` and try to figure out, what to do next. The key is `(\b.bb)(\v.vv)06FHPVboundvarHRAk`.
* `sandmark-d.log` contains the disassembly log for reference inmplementation, working on `sandmark.umz`;
`sandmark-v.log` additionally contains register dump. The same is true for `codex-*.log`.

Makefile is present in order to simplify running

- make exec -- just for the executable `exec`
- make sandmark -- compile and run sandmark
- make sandmark_debug -- compile with debug output and run sandmark (debug output is written to /tmp/um_log)
- make codex -- compile and run codex with automatically entered password
- make clean
