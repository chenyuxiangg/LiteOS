CURRENT_PATH=$(cd $(dirname $0); pwd)
ROOT_PATH=${CURRENT_PATH}/../..
BUILD_PATH=${ROOT_PATH}/build
OUTPUT_PATH=${ROOT_PATH}/out/qemu-virt-riscv

QEMU=/home/zyzs/code/osz/osz/tools/qemu/bin/qemu-system-riscv32
QFLAGS="-nographic -smp 1 -machine virt -bios none -m 128M"
GDB=/home/zyzs/code/osz/osz/tools/riscv32-unknown-elf/bin/riscv32-unknown-elf-gdb

echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
echo "Press Ctrl-A-X to quit QEMU"
echo "-------------------------------------------------------"
${BUILD_PATH}/scripts/kill_qemu.sh
${QEMU} ${QFLAGS} -kernel ${OUTPUT_PATH}/Huawei_LiteOS.elf