#!/usr/bin/bash

NUMCPUS=`../../WebKitTools/Scripts/num-cpus`

WEBKITROOT="$1"
export WEBKITROOT
# Do a little dance to get the path into 8.3 form to make it safe for gnu make
# http://bugzilla.opendarwin.org/show_bug.cgi?id=8173
WEBKITROOT=`cygpath -m -s "$WEBKITROOT"`
WEBKITROOT=`cygpath -u "$WEBKITROOT"`
export WEBKITROOT

export XSRCROOT="${WEBKITROOT}/WebCore"
export SOURCE_ROOT=$XSRCROOT

export BUILT_PRODUCTS_DIR="$WEBKITROOT/iSee/build"
cd "${WEBKITROOT}/iSee/tmp"

export CREATE_HASH_TABLE="$WEBKITROOT/JavaScriptCore/kjs/create_hash_table"

export WebCore="${XSRCROOT}"
export FEATURE_DEFINES="ENABLE_DATABASE ENABLE_ICONDATABASE ENABLE_XPATH" # ENABLE_SVG ENABLE_SVG_FONTS ENABLE_SVG_AS_IMAGE ENABLE_SVG_USE ENABLE_VIDEO
make -f "$WebCore/DerivedSources.make" -j ${NUMCPUS} || exit 1
