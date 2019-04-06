#!/bin/bash

# generate template code files

cp DIRECTORY-MONITOR-MIB.txt ~/.snmp/mibs/
echo "mibs +DIRECTORY-MONITOR-MIB" > ~/.snmp/snmp.conf

rm -rf template
mkdir template

SCALARS="Directory NumFiles NumDirectories"
TABLES="DirectoryContentTable"
NOTIFICATIONS="DirectoryStateNotification"

cd ./template

# generating scalars
for FILE in $SCALARS
do
    echo "generating" $FILE "..."
    mib2c -c mib2c.scalar.conf $FILE
    echo ""
done

# generating tables
for FILE in $TABLES
do
    echo "generating" $FILE "..."
    mib2c -c mib2c.mfd.conf $FILE
    echo ""
done

# generating notifications
for FILE in $NOTIFICATIONS
do
    echo "generating" $FILE "..."
    mib2c -c mib2c.notify.conf $FILE
    echo ""
done
