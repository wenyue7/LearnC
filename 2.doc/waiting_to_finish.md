以udoo-neo-full 为硬件平台的简单示例：
git://git.busybox.net/buildroot
make list-defconfigs | grep udoo
make mx6sx_udoo_neo_defconfig
make all
# 第八章 yocto构建Linux系统



