#!/usr/bin/bash

NUMCPUS=`../../WebKitTools/Scripts/num-cpus`

WEBKITROOT="$1"
export WEBKITROOT
# Do a little dance to get the path into 8.3 form to make it safe for gnu make
# http://bugzilla.opendarwin.org/show_bug.cgi?id=8173
WEBKITROOT=`cygpath -m -s "$WEBKITROOT"`
WEBKITROOT=`cygpath -u "$WEBKITROOT"`
export WEBKITROOT

export XSRCROOT="${WEBKITROOT}/JavaScriptCore"
export SOURCE_ROOT=$XSRCROOT

export BUILT_PRODUCTS_DIR="$WEBKITROOT/iSee/build"
cd "${WEBKITROOT}/iSee/tmp"

export JavaScriptCore="${XSRCROOT}"
make -f "$JavaScriptCore/DerivedSources.make" -j ${NUMCPUS} || exit 1
