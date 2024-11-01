#!/bin/bash

# Check if an assignment number is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <assignment_number>"
    exit 1
fi

ASSIGNMENT_NUMBER="$1"
SOURCE_DIR="output_files"
VALIDATION_DIR="validation"

case "$ASSIGNMENT_NUMBER" in
    2)
        PATTERN="c_tokens.txt"
        ;;
    3)
        PATTERN="c_pathed.txt"
        ;;
    4)
        PATTERN="c_symbols.txt"
        ;;
    *)
        echo "No file patterns defined for assignment number: $ASSIGNMENT_NUMBER"
        exit 1
        ;;
esac


for SOURCE_FILE in ${SOURCE_DIR}/programming_assignment_${ASSIGNMENT_NUMBER}-test_file_*.${PATTERN}; do
    if [ -f "$SOURCE_FILE" ]; then
        # Extract the base name of the source file to find corresponding validation files
        BASE_NAME=$(basename "$SOURCE_FILE" "${PATTERN}")

        # Construct the corresponding validation file name
	VALIDATION_FILE="${VALIDATION_DIR}/output-programming_assignment_${ASSIGNMENT_NUMBER}-${BASE_NAME##*-}txt"


        # Check if the validation file exists
        if [ -f "$VALIDATION_FILE" ]; then
	    if ! diff -q "$SOURCE_FILE" "$VALIDATION_FILE" > /dev/null; then
                DIFF_COUNT=$((DIFF_COUNT + 1))
                echo "Difference found for $SOURCE_FILE."
            fi
            #echo "Diffing $SOURCE_FILE with $VALIDATION_FILE:"
            #diff "$SOURCE_FILE" "$VALIDATION_FILE" || echo "Differences found."
        else
            echo "No validation files: ${VALIDATION_FILE}"
        fi
    else
        echo "No source files matching pattern: ${SOURCE_DIR}/programming_assignment_${ASSIGNMENT_NUMBER}-test_file_*.${PATTERN}."
    fi
done
