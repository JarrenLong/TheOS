#include "multiboot.h"

#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))
#define COLUMNS			80
#define LINES			24
#define ATTRIBUTE		7
#define VIDEO			0xB8000

static int xpos;
static int ypos;
static volatile unsigned char *video;

void cmain (unsigned long magic, unsigned long addr);
static void cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void printf (const char *format, ...);

void cmain (unsigned long magic, unsigned long addr) {
    multiboot_info_t *mbi;
    cls ();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
        return;
    }
    mbi = (multiboot_info_t *) addr;
    printf ("flags = 0x%x\n", (unsigned) mbi->flags);
    if (CHECK_FLAG (mbi->flags, 0))
        printf ("mem_lower = %uKB, mem_upper = %uKB\n",
                (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
    if (CHECK_FLAG (mbi->flags, 1))
        printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);
    if (CHECK_FLAG (mbi->flags, 2))
        printf ("cmdline = %s\n", (char *) mbi->cmdline);
    if (CHECK_FLAG (mbi->flags, 3)) {
        multiboot_module_t *mod;
        int i;
        printf ("mods_count = %d, mods_addr = 0x%x\n",
                (int) mbi->mods_count, (int) mbi->mods_addr);
        for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
                i < mbi->mods_count;
                i++, mod++)
            printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                    (unsigned) mod->mod_start,
                    (unsigned) mod->mod_end,
                    (char *) mod->cmdline);
    }
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5)) {
        printf ("Both bits 4 and 5 are set.\n");
        return;
    }
    if (CHECK_FLAG (mbi->flags, 4)) {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
        printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
                "strsize = 0x%x, addr = 0x%x\n",
                (unsigned) multiboot_aout_sym->tabsize,
                (unsigned) multiboot_aout_sym->strsize,
                (unsigned) multiboot_aout_sym->addr);
    }
    if (CHECK_FLAG (mbi->flags, 5)) {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);
        printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
                " addr = 0x%x, shndx = 0x%x\n",
                (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
                (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }
    if (CHECK_FLAG (mbi->flags, 6)) {
        multiboot_memory_map_t *mmap;
        printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
                (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                                   + mmap->size + sizeof (mmap->size)))
            printf (" size = 0x%x, base_addr = 0x%x%x,"
                    " length = 0x%x%x, type = 0x%x\n",
                    (unsigned) mmap->size,
                    mmap->addr >> 32,
                    mmap->addr & 0xffffffff,
                    mmap->len >> 32,
                    mmap->len & 0xffffffff,
                    (unsigned) mmap->type);
    }
}

static void
cls (void) {
    int i;
    video = (unsigned char *) VIDEO;
    for (i = 0; i < COLUMNS * LINES * 2; i++)
        *(video + i) = 0;
    xpos = 0;
    ypos = 0;
}

static void
itoa (char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    if (base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 'x')
        divisor = 16;
    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);
    *p = 0;
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

static void
putchar (int c) {
    if (c == '\n' || c == '\r') {
newline:
        xpos = 0;
        ypos++;
        if (ypos >= LINES)
            ypos = 0;
        return;
    }
    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
    xpos++;
    if (xpos >= COLUMNS)
        goto newline;
}

void
printf (const char *format, ...) {
    char **arg = (char **) &format;
    int c;
    char buf[20];
    arg++;
    while ((c = *format++) != 0) {
        if (c != '%')
            putchar (c);
        else {
            char *p;
            c = *format++;
            switch (c) {
            case 'd':
            case 'u':
            case 'x':
                itoa (buf, c, *((int *) arg++));
                p = buf;
                goto string;
                break;
            case 's':
                p = *arg++;
                if (! p)
                    p = "(null)";
string:
                while (*p)
                    putchar (*p++);
                break;
            default:
                putchar (*((int *) arg++));
                break;
            }
        }
    }
}
