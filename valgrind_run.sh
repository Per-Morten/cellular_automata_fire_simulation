valgrind_args="--suppressions=min.suppressions --leak-check=full --show-leak-kinds=reachable --read-var-info=yes --track-origins=yes --expensive-definedness-checks=yes --keep-stacktraces=alloc-and-free"

if [[ $# -lt 2 ]]
    then
    valgrind ${valgrind_args} ${1}
else
    if [[ ${2} =~ "-P" ]]
        then
        valgrind ${valgrind_args} ${1} 2> /dev/null
    elif [[ ${2} =~ "-V" ]]
        then
        valgrind ${valgrind_args} ${1} > /dev/null
    fi
fi

