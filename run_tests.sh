#!/bin/bash

# set -e

# Define the target directory
target_dir="test"

# Check if the directory exists
if [ ! -d "$target_dir" ]; then
    echo "Directory '$target_dir' not found."
    exit 1
fi

make

# check bad parses
find "$target_dir" -type f -name "bad_parse*.tiger" | while read -r file; do
    echo -e "\e[32mProcessing file: $file\e[0m"
    ./cs8803_bin/tigerc -f "$file" -s -p -v
    if [ $? -ne 3 ]; then
        echo -e "\e[31mtigerc command failed on file: $file\e[0m"
    fi
done

# check bad scans
find "$target_dir" -type f -name "bad_scan*.tiger" | while read -r file; do
    echo -e "\e[32mProcessing file: $file\e[0m"
    ./cs8803_bin/tigerc -f "$file" -s -p -v
    if [ $? -ne 2 ]; then
        echo -e "\e[31mtigerc command failed on file: $file\e[0m"
    fi
done

# check valid files
find "$target_dir" -type f -name "*.tiger" -not -name "bad_*" | while read -r file; do
    echo -e "\e[32mProcessing file: $file\e[0m"
    echo "Processing file: $file"
    ./cs8803_bin/tigerc -f "$file" -s -p -v
    if [ $? -ne 0 ]; then
        echo -e "\e[31mtigerc command failed on file: $file\e[0m"
    fi
done