#!/bin/sh
# exifautotran [list of files]
#
# Transforms Exif files so that Orientation becomes 1
#
for i
do
 case $i in
 -v|--version) echo "exifautotran"; exit 0;;
 -h|--help) 
             cat <<EOF
exifautotran [list of files]

Transforms Exif files so that Orientation becomes 1
EOF
             exit 0;;
 esac

 case `jpegexiforient -n "$i"` in
 1) transform="";;
 2) transform="-flip horizontal";;
 3) transform="-rotate 180";;
 4) transform="-flip vertical";;
 5) transform="-transpose";;
 6) transform="-rotate 90";;
 7) transform="-transverse";;
 8) transform="-rotate 270";;
 *) transform="";;
 esac
 if test -n "$transform"; then
  echo Executing: jpegtran -copy all $transform $i >&2
  jpegtran -copy all $transform "$i" > tempfile
  if test $? -ne 0; then
   echo Error while transforming $i - skipped. >&2
  else
   rm "$i"
   mv tempfile "$i"
   jpegexiforient -1 "$i" > /dev/null
  fi
 fi
done
