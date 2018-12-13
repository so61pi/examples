#!/bin/bash
#http://tldp.org/LDP/abs/html/string-manipulation.html

string="value"

echo "length    : ${#string}"       # 5
echo "concat    : $string-1"        # value-1
echo "concat    : $string$string"   # valuevalue
echo "substring : ${string:2}"      # lue
echo "substring : ${string:2:2}"    # lu

# string is value
if [ "$string" = "value" ]; then
    echo "string is value"
else
    echo "string is not value"
fi

# string is value
if [ "${string}-x" = "value-x" ]; then
    echo "string is value"
else
    echo "string is not value"
fi

# str is not value
if [ "${str}-x" = "value-x" ]; then
    echo "str is value"
else
    echo "str is not value"
fi
