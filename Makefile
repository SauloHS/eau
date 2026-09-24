CC := gcc
LD := ld

CFLAGS := -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check \
          -fno-pie -fno-pic -m64 -march=x86-64 -mno-80387 -mno-mmx \
          -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -Wall -Wextra -I.

LDFLAGS := -m elf_x86_64 -nostdlib -static -no-pie \
           -T linker.ld -z max-page-size=0x1000

SRC := $(shell find src drivers utils -name '*.c')
OBJ := $(SRC:.c=.o)

KERNEL := kernel

.PHONY: all clean run

all: $(KERNEL)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(KERNEL)

iso: $(KERNEL)
	mkdir -p iso_root/boot
	cp $(KERNEL) iso_root/boot/kernel
	mkdir -p iso_root/boot/limine
	cp limine.conf iso_root/boot/limine/
	cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/boot/limine/
	mkdir -p iso_root/EFI/BOOT
	cp limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp limine/BOOTIA32.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		-hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o eau.iso
	./limine/limine bios-install eau.iso

run: iso
	qemu-system-x86_64 -cdrom eau.iso -m 512M -serial stdio

clean:
	rm -rf $(OBJ) $(KERNEL) iso_root eau.iso
