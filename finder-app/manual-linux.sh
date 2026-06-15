#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-
SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR}

# directory creation

mkdir -p "$OUTDIR/rootfs"
cd "$OUTDIR/rootfs"
mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib usr/sbin
mkdir -p var/log
mkdir -p home/conf

# TODO: Add library dependencies to rootfs

SYSROOT_DIR=$(${CROSS_COMPILE}gcc -print-sysroot)

cp $SYSROOT_DIR/lib/ld-linux-aarch64.so.1 $OUTDIR/rootfs/lib

cp $SYSROOT_DIR/lib64/{libm.so.6,libresolv.so.2,libc.so.6} $OUTDIR/rootfs/lib64


cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} mrproper
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} defconfig
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} all
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} modules
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} dtbs
    # TODO: Add your kernel build steps here
fi

echo "Adding the Image in outdir"

cp ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ${OUTDIR}

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs
fi

# directory creation

mkdir -p "$OUTDIR/rootfs"
cd "$OUTDIR/rootfs"
mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib usr/sbin
mkdir -p var/log
mkdir -p home/conf



cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
    
    until git clone git://busybox.net/busybox.git --depth 1 --branch ${BUSYBOX_VERSION}
    do 
        echo "git clone failed"
    done
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    make distclean
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}  defconfig
    # TODO:  Configure busybox
else
    cd busybox
fi

make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}
make -j4 CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install

echo "Library dependencies"
${CROSS_COMPILE}readelf -a $OUTDIR/rootfs/bin/busybox | grep "program interpreter"
${CROSS_COMPILE}readelf -a $OUTDIR/rootfs/bin/busybox | grep "Shared library"

# TODO: Make device nodes

sudo mknod -m 666 ${OUTDIR}/rootfs/dev/null c 1 3
sudo mknod -m 666 ${OUTDIR}/rootfs/dev/console c 5 1

# TODO: Clean and build the writer utility
echo "clean and build"

cd "$FINDER_APP_DIR"
make clean
make CROSS_COMPILE=${CROSS_COMPILE} all

# move writer application
echo "move writer"
cp $FINDER_APP_DIR/writer "$OUTDIR/rootfs/home"

# TODO: Copy the finder related scripts and executables to the /home directory
# on the target rootfs
echo "move scripts and execs"
cp $FINDER_APP_DIR/{finder.sh,finder-test.sh,autorun-qemu.sh} $OUTDIR/rootfs/home
cp $FINDER_APP_DIR/{username.txt,assignment.txt} $OUTDIR/rootfs/home/conf

# TODO: Chown the root directory
sudo chown 777 $OUTDIR/rootfs

# TODO: Create initramfs.cpio.gz
echo "initramfs"
cd "$OUTDIR/rootfs"
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
cd "$OUTDIR"
gzip -f initramfs.cpio