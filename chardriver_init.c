#include "linux/module.h"         // Core header for loading LKMs into the kernel
#include "linux/device.h"         // Header to support the kernel Driver Model
#include "linux/kernel.h"         // Contains types, macros, functions for the kernel
#include "linux/fs.h"             // Header for the Linux file system support
#include "asm/uaccess.h"          // Required for the copy to user function
#include "init.h"                 // Macros used to mark up functions e.g. __init __exit





