
DEVKITPRO=/opt/devkitpro
CTRPFDIR=$DEVKITPRO/libctrpf

rm -rf $CTRPFDIR
mkdir -p $CTRPFDIR
bzip2 -cd $1 | tar -xf - -C $CTRPFDIR
echo "CTRPF was successfully installed in $CTRPFDIR"
