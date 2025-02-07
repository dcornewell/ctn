#!/bin/csh

# Setup for runtime directory.  Make sure it exists.

if ($1 == "") then
  echo "You need to define the environment variable CTN_TARGET."
  echo "The default value is /opt/ctn, but you can use a different value."
  exit 1
endif

$DICOM_ROOT/apps/scripts/createdirectory $CTN_TARGET
if ($status != 0) exit 1

$DICOM_ROOT/apps/scripts/createdirectory $CTN_TARGET/runtime
if ($status != 0) exit 1
