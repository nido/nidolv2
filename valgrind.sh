#!/bin/bash
make check && /usr/bin/valgrind -v --leak-check=full --show-reachable=yes nidoamp/test_nidoamp || gdb nidoamp/test_nidoamp
