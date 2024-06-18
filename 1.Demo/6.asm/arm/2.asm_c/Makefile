NDK_ROOT=$(shell echo ${HOME})/work/android/ndk/android-ndk-r25c
CLANG_TOOL=$(NDK_ROOT)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang

all:
	$(info NDK ROOT ==> $(NDK_ROOT))
	$(info CLANG    ==> $(CLANG_TOOL))
	$(CLANG_TOOL) -o main main.c

clean:
	rm main
