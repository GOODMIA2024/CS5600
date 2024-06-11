#!/bin/bash

# Compile the program
make
echo "Compiled the program successfully"

for i in -u -l -cap -x
do 
    ./arger-tao $i "help is on the way"
    if test $? -eq 0 
    then # Note the spaces after [ and before ]
        echo "Test of $i passed\n"
    else
        echo "Test of $i failed\n"
    fi
    # Debug: echo "finish $i"
done

echo "Testing finished"
# ./arger-tao -u "help is on the way"
# # Check and make sure somecomm and returns expected code (0 in this case)


# ./arger-tao -l "help is on the way"
# if test $? -eq 0 
# then  # 为什么需要中括号
#     echo "Test of -l passed"
# else
#     echo "Test of -l failed"
# fi


# ./arger-tao -cap "help is on the way"
# if test $? -eq 0 
# then  # 为什么需要中括号
#     echo "Test of -cap passed"
# else
#     echo "Test of -cap failed"
# fi

# ./arger-tao -c "help is on the way"
# if test $? -eq 0 
# then  # 为什么需要中括号
#     echo "Test of -cap passed"
# else
#     echo "Test of -cap failed"
# fi

echo "All tests completed."

