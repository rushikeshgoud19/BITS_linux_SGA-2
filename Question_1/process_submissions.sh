#!/bin/bash
# process_submissions.sh
# Identifies duplicate submissions, backs up unique ones, and generates a report

SUBMISSION_DIR="submissions"
BACKUP_DIR="backup"
REPORT="report.txt"
ERRORS="errors.log"

mkdir -p "$BACKUP_DIR" 2>> "$ERRORS"
> "$REPORT"
> "$ERRORS"

declare -A seen_hashes
processed=0
duplicates=0
backed_up=0

for file in "$SUBMISSION_DIR"/*; do
    if [ -f "$file" ]; then
        ((processed++))

        file_hash=$(md5sum "$file" 2>> "$ERRORS" | awk '{print $1}')

        if [[ -n "${seen_hashes[$file_hash]}" ]]; then
            ((duplicates++))
        else
            seen_hashes[$file_hash]=1
            cp "$file" "$BACKUP_DIR/" 2>> "$ERRORS"
            ((backed_up++))
        fi
    fi
done

echo "Files Processed: $processed" > "$REPORT"
echo "Duplicates Found: $duplicates" >> "$REPORT"
echo "Files Backed Up (Unique): $backed_up" >> "$REPORT"

echo "Processing complete. Check $REPORT and $ERRORS."
