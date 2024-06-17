all:
	arm-none-eabi-as -o hello.o hello.s
	arm-none-eabi-ld hello.o -o hello.elf
	# （可选）如果你有一个模拟器或加载器，你可以使用它来运行hello.elf
	# 例如，使用qemu模拟ARM处理器：
	# qemu-system-arm -M your-machine-type -kernel hello.elf

clean:
	rm hello.o hello.elf
