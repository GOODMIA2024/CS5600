#!/bin/bash

# Test when the getRand program cannot be found
make clean
./getRand 1000 random_words.txt
if [ $? -eq 0 ]; then
    echo "Error: 'getRand' program somehow exists."
else
    ./multiprocessing
    if [ $? -eq 0 ]; then
        echo "Error: 'multiprocessing' somehow exists."
    else
        echo "Success: 'getRand' and 'multiprocessing' program not found."
    fi
fi
make clean


# Test 0 word
make
./getRand 0 random_words.txt
if [ $? -eq 0 ]; then
    ./multiprocessing
    if [ $? -eq 0 ]; then
        if [ -z "$(find . -maxdepth 1 -type f -name 'output_batch*' -print -quit)" ]; then
            echo "Success: No output file is created since the input file is empty."
        else
            echo "Error: Some output files are created despite the input file is empty."
        fi
    else
        echo "Error: 'multiprocessing' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 1000 words
make
./getRand 1000 random_words.txt
if [ $? -eq 0 ]; then
    ./multiprocessing
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 10 ]; then
            echo "Success: There are 10 output files for 1000 words."
        else
            echo "Error: There are not 10 output files for 1000 words."
        fi
    else
        echo "Error: 'multiprocessing' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 1999 words
make
./getRand 1999 random_words.txt
if [ $? -eq 0 ]; then
    ./multiprocessing
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 20 ]; then
            echo "Success: There are 20 output files for 1999 words."
        else
            echo "Error: There are not 20 output files for 1999 words."
        fi
    else
        echo "Error: 'multiprocessing' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 2023 words
make
./getRand 2023 random_words.txt
if [ $? -eq 0 ]; then
    ./multiprocessing
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 21 ]; then
            echo "Success: There are 21 output files for 2023 words."
        else
            echo "Error: There are not 21 output files for 2023 words."
        fi
    else
        echo "Error: 'multiprocessing' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 10000 words
make
./getRand 10000 random_words.txt
if [ $? -eq 0 ]; then
    ./multiprocessing
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 100 ]; then
            echo "Success: There are 100 output files for 10000 words."
        else
            echo "Error: There are not 100 output files for 10000 words."
        fi
    else
        echo "Error: 'multiprocessing' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean