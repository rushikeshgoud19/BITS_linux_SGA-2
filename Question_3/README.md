# Question 3 — Secure Employee Record File Utility (Raw Syscalls)

## Commands Executed
```
gcc secure_db.c -o secure_db
./secure_db
```

## Output (verified)
```
Initial records written.
Record 2 updated.
Retrieved Record: ID=1, Name=Alice, Salary=50000.00
```

**Note on the last line:** the program reads back the record at offset 0, which is Alice (record 1) — that's correct behavior, not a bug. The update in the previous step was written to record 2's offset, it just isn't the one being read back here.

## Explanations

**`gcc secure_db.c -o secure_db`** — Compiled the program using low-level POSIX syscalls (`<fcntl.h>`, `<unistd.h>`) instead of buffered `stdio` functions like `fopen`/`fwrite`.

**`./secure_db`** — Running it created `database.dat`, wrote two fixed-size employee records, updated the second record's salary in place using a direct offset (no full-file rewrite), then seeked back to the start and read the first record back out to confirm the data round-trips correctly.

## Technique Justification

* **`open()` / `close()`** — `open()` with `O_CREAT | O_RDWR` and mode `0600` creates the file if it doesn't exist and restricts access to the owner only (read/write, no group/other access) — appropriate for a "secure" utility handling employee data. `close()` releases the file descriptor once done.
* **`write()`** — Writes the raw fixed-size `Employee` struct directly to disk at the current file offset, byte-for-byte, without the extra buffering layer `fprintf`/`fwrite` would add.
* **`lseek()`** — Moves the file offset to `sizeof(Employee) * index` to jump straight to any record's exact byte position. This is what makes in-place updates and random-access reads possible without loading or rewriting the whole file — critical for the "update specific records without rewriting the entire file" requirement.
* **`read()`** — Reads exactly `sizeof(Employee)` bytes starting from the current offset into a struct, letting any record be retrieved directly by seeking to its position first.

## How it fits together
Because every record is a fixed size (`sizeof(Employee)`), `lseek()` can calculate any record's exact byte offset (`index * sizeof(Employee)`), making the file behave like a simple flat-file database with O(1) access to any record — updates and reads never require touching unrelated records.

## Files in this folder
- `secure_db.c` — the program
- `README.md` — this file
- `Q3_screenshot.png` — terminal screenshot (add your own)
