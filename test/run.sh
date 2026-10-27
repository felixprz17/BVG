#!/bin/sh
# Host-Tests (PC, g++ >= 7): Logik wie Zeit, BVG-Parsing, Cache, Zeilenaufbau
cd "$(dirname "$0")"
python3 gen_cases.py cases.tsv || exit 1
g++ -std=c++17 -Wall -Wextra -Ivendor -o host_tests host_tests.cpp && ./host_tests
