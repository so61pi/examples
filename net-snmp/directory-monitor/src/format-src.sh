#!/bin/bash

CLANGFORMAT=clang-format-3.6
SRCS="monitor-agent.c
      Directory.c Directory.h
      DirectoryStateNotification.c DirectoryStateNotification.h
      NumDirectories.c NumDirectories.h
      NumFiles.c NumFiles.h
      Utility.c Utility.h
      GlobalSettings.c GlobalSettings.h
      DirectoryContentTable.c DirectoryContentTable.h
      DirectoryContentTable_data_access.c DirectoryContentTable_data_access.h
      DirectoryContentTable_data_get.c DirectoryContentTable_data_get.h
      DirectoryContentTable_data_set.c DirectoryContentTable_data_set.h
      DirectoryContentTable_enums.h
      DirectoryContentTable_interface.c DirectoryContentTable_interface.h
      DirectoryContentTable_oids.h"

for FILE in $SRCS
do
    echo "Formating" $FILE
    $CLANGFORMAT -style=file -i $FILE
done
