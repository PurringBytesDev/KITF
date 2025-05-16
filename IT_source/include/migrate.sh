#!/bin/bash

# === Configuration ===
SOURCE_DIR="."
DEST_DIR="output"
EXTENSION=".h"

# === Create destination folder ===
mkdir -p "$DEST_DIR"

# === Check if source folder exists ===
if [ ! -d "$SOURCE_DIR" ]; then
    echo "ERROR: Source directory '$SOURCE_DIR' not found!"
    exit 1
fi

# === Find all .h files ===
echo "Searching for .h files in $SOURCE_DIR..."

FOUND=0
for HEADER_FILE in $SOURCE_DIR/*$EXTENSION; do
    if [ -f "$HEADER_FILE" ]; then
        FOUND=1
        FILE_NAME=$(basename "$HEADER_FILE" .h)
        DEST_HEADER="$DEST_DIR/$FILE_NAME.h"
        DEST_CPP="$DEST_DIR/$FILE_NAME.cpp"

        echo "Processing $HEADER_FILE..."

        # Copy the header to the output folder
        cp "$HEADER_FILE" "$DEST_HEADER"

        # Extract method implementations to .cpp
        while IFS= read -r LINE; do
            if [[ "$LINE" == *"{"* ]]; then
                echo "$LINE" >> "$DEST_CPP"
            fi
        done < "$HEADER_FILE"
    fi

done

if [ "$FOUND" -eq 0 ]; then
    echo "No .h files found in $SOURCE_DIR!"
    exit 1
fi

echo "=== Processing Complete ==="
