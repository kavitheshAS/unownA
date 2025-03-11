## Overview
1. set up the cross compiler
2. setup grub to boot into the custom os or alternatively write a boot.s file that will have all the instructions, like creating the stack, multiboot headers etc

The assembly file (boot.s) is responsible for:

- Declaring a Multiboot header
    This allows the bootloader (like GRUB) to recognize and correctly load the kernel.
    It includes a magic number (0x1BADB002), flags, and a checksum to validate the header.

- Setting up a minimal stack
    Since the CPU does not provide a stack when jumping to the kernel, this code reserves space (16 KB) and sets the ESP register to point to the top of it.

- Providing an entry point (_start)
    This is where execution begins when the bootloader transfers control to the kernel.
    It sets up the stack and prepares the CPU for higher-level kernel execution.

- Jumping to kernel_main
    The function kernel_main (which should be implemented in C) will be called next, marking the transition from assembly to higher-level code.

- Entering an infinite loop
    If kernel_main returns (which it shouldn’t), the system enters an infinite hlt (halt) loop to prevent it from executing invalid code.