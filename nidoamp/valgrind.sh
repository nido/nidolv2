#!/bin/bash -x
pwd
/usr/bin/valgrind -v --leak-check=full --show-reachable=yes ./test_nidoamp 2>&1 | grep "\(blocks.*lost\|by 0x\|at 0x\|ERROR SUMMARY\)"
