#!/bin/sh

SCRIPTDIR=`dirname $0`
ROOTDIR=`(cd $SCRIPTDIR/../.. ; pwd)`
VERSION=`cat VERSION.txt | sed 's/^ *//;s/ *$//'` # make sure to trim whitespace

PACKAGE="$1"
if [ -z "$PACKAGE" ] ; then
  echo "Usage: $0 package-name"
  exit 1
fi

FILELIST="$ROOTDIR/dist/releasefiles-$PACKAGE.txt"
if [ -e "$FILELIST" ] ; then
  echo "Creating package from $FILELIST"
else
  echo "Not a known package: $PACKAGE"
  exit 1
fi


INST_ROOTDIR="SEMAINE-$VERSION"


mkdir -p "$ROOTDIR/dist/$INST_ROOTDIR"
DISTFILE="$ROOTDIR/dist/$INST_ROOTDIR/$INST_ROOTDIR-$PACKAGE.zip"
if [ -e "$DISTFILE" ] ; then
  echo "Removing existing $DISTFILE"
  rm "$DISTFILE"
fi

TMPDIR="$ROOTDIR/dist/tmp"
mkdir -p "$TMPDIR"
if [ -e "$TMPDIR/$INST_ROOTDIR" ] ; then
  echo "Removing existing $TMPDIR/$INST_ROOTDIR"
  rm "$TMPDIR/$INST_ROOTDIR"
fi
ln -s "$ROOTDIR" "$TMPDIR/$INST_ROOTDIR"
DISTFILELIST="$TMPDIR/distfiles-$PACKAGE.txt"
if [ -e "$DISTFILELIST" ] ; then
  echo "Removing existing $DISTFILELIST"
  rm "$DISTFILELIST"
fi

# Prepare the actual list of files from $FILELIST:
# prepend $INST_ROOTDIR in front of each file, 
# and expand directories.

echo "Creating full file list in $DISTFILELIST"

# In argument of for loop, split at newline only,
# in order to allow file names including spaces:
OLDIFS="$IFS"
IFS='
'

(cd "$TMPDIR" 
for f in `cat "$FILELIST"` ; do
  if [ -d "$INST_ROOTDIR/$f" ] ; then
    find "$INST_ROOTDIR/$f" -print | egrep -v "^\.svn|/\.svn|~$|/\.libs|\.o$|\.lo$|\.loT$|.DS_Store|/\.deps|\.a$|/\.dirstamp$" >> "$DISTFILELIST" 
  elif [ -f "$INST_ROOTDIR/$f" ] ; then
    echo "$INST_ROOTDIR/$f" >> "$DISTFILELIST"
  else
    echo "Warning: $ROOTDIR/$f does not exist."
  fi
done

echo "Creating $DISTFILE"

cat "$DISTFILELIST" | zip -q "$DISTFILE" -@
)

IFS="$OLDIFS"

echo "Created $DISTFILE"

