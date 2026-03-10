#!/bin/bash

make

(echo "ls"; echo "exit") | ./minishell > output.txt
if grep -q "Makefile" output.txt; then
    echo "Test 1 Proso: ls Radi"
else
    echo "Test 1 nije proso!"
    exit 1
fi

(echo "pwd"; echo "exit") | ./minishell > output.txt
if grep -q "/" output.txt; then
    echo "Test 2 Proso: pwd Radi"
else
    echo "Test 2 nije proso"
    exit 1
fi

(echo "ps | head 3"; echo "exit") | ./minishell > output.txt
if grep -q "/" output.txt; then
    echo "Test 3 Proso: ps | head 3 Radi"
else
    echo "Test 2 nije proso"
    exit 1
fi

echo "Svi testovi su uspesno zavrseni!"