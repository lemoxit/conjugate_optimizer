###
 # @Author: your name
 # @Date: 2020-04-28 23:30:30
 # @LastEditTime: 2020-04-28 23:31:18
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /conjugate_optimizer/format.sh
 ###
#!/bin/bash
echo -n "Running clang-format "
find . -name "*\.h" -o -name "*\.cc"|grep -v bundled|xargs -I {} sh -c "clang-format -i -style=Google {}; echo -n '.'"
echo


