#!/bin/sh

# "Make" already run test after compiling, so we have to exit
test -f "test" || (make >> /dev/null && exit)

output=$(echo "string" | ./test 123 321 arg)
except=$(cat << EOF
> Arguments count
4
> Arguments\` variables
./test
123
321
arg
> Enter string
> First character from this file
*
> What remains in file
 if you can see it, fprint worked *
string
> Check log file
EOF
)

([[ $output = $except ]] && echo -e "\x1b[32mTest passed\033[0m") ||
! echo -e "\x1b[31mTest failed\033[0m"
