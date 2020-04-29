#!/bin/bash
echo -n "Running clang-format "
find . -name "*\.h" -o -name "*\.cc"|grep -v bundled|xargs -I {} sh -c "clang-format-3.9 -i -style=Google {}; echo -n '.'"
echo


