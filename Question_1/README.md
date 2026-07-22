# Question 1 — Duplicate Submission Detection & Backup Script

## Commands Executed
```
mkdir submissions && touch submissions/file1.txt submissions/file2.txt submissions/file3.txt
echo "hello" > submissions/file1.txt; echo "hello" > submissions/file2.txt; echo "world" > submissions/file3.txt
chmod +x process_submissions.sh
./process_submissions.sh
cat report.txt
```

## Output (verified)
```
Files Processed: 3
Duplicates Found: 1
Files Backed Up (Unique): 2
```
`backup/` ends up containing `file1.txt` and `file3.txt` — `file2.txt` was skipped because its content hash matched `file1.txt`.

## Explanations

**`mkdir submissions && touch ...`** — I created the working directory and three sample submission files to simulate what a batch of student uploads would look like.

**`echo "hello" > file1.txt / file2.txt`, `echo "world" > file3.txt`** — I deliberately gave two files identical content so I could confirm the duplicate-detection logic actually catches a match rather than just running without errors.

**`chmod +x process_submissions.sh`** — I made the script executable so it can be run directly with `./process_submissions.sh` instead of calling `bash process_submissions.sh`.

**`./process_submissions.sh`** — Running the script processed all files in `submissions/`, detected the one duplicate, and copied only the two unique files into `backup/`.

**`cat report.txt`** — I inspected the report to confirm the counts (3 processed, 1 duplicate, 2 backed up) matched what I expected from the test data.

## Technique Justification

* **Hash generation (`md5sum "$file" | awk '{print $1}'`)** — I used `md5sum` to fingerprint file *content* rather than filenames, so two submissions with different names but identical content are still caught as duplicates. `awk` strips out the filename `md5sum` prints alongside the hash, leaving just the hash string for comparison.
* **Associative array (`declare -A seen_hashes`)** — Used as a fast lookup table (hash → seen/not seen) so checking for a duplicate is O(1) instead of re-scanning the backup folder for every file.
* **Error redirection (`2>> "$ERRORS"`)** — I appended standard error (file descriptor 2) to a separate `errors.log` so permission issues or missing-file errors don't clutter the console or get mixed into the report.
* **Output redirection (`>` vs `>>`)** — I used `>` once at the start to create/clear `report.txt` and `errors.log` for a fresh run, then `>>` for every subsequent write so lines accumulate instead of overwriting each other.
* **`mkdir -p`** — Used so the script doesn't error out if `backup/` already exists from a previous run.

## Files in this folder
- `process_submissions.sh` — the script
- `README.md` — this file
- `Q1_screenshot.png` — terminal screenshot (add your own)
