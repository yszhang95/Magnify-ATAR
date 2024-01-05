#!/bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

rootfile="$1" ; shift
if [[ "$rootfile" =~ :// ]] ; then
    echo "Loading URL $rootfile"
else
    rootfile=$(readlink -f "$rootfile")
fi
# sign="${1:-0}"
# startdir=$(pwd)


cd $magnify_source/scripts

if [[ $rootfile ]]; then
    root -l loadClasses.C Magnify.C'("'"$rootfile"'")'
else
    root -l loadClasses.C Magnify.C
fi


