#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "you arent using a cross compiler, you will most likely run into trouwle"
#endif

#if !defined(__1386__)
#error "this must be compiled with a ix86-elf compiler"
#endif

/*hardware text mode color constants*/

enum vga_color{
    vga_black=0,
    vga_blue=1,
    vga_green=2,
    vga_cyan=3,
    vga_red=4,
    vga_magenta=5,
    vga_brown=6,
    vga_light_grey=7,
    vga_dark_grey=8,
    vga_light_blue=9,
    vga_light_green=10,
    vga_light_cyan=11,
    vga_light_red=12,
    vga_light_magenta=13,
    vga_light_brown=14,
    vga_white=15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg <<4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
    return (uint16_t) uc | (uint16_t) color << 8;
}

//the inconsistency in the return statements on the 2 functions is because the first one doesnt need typecasting , while second one does
//why? because the first one is not prone to overflow, but in 2nd case the uc is by default 8 bits , so if we dont force it to be 16 bits, when done <<8
//it will overflow and give wrong results

size_t strlen(const char* str){
    size_t len=0;
    while(str[len]) len++;
    return len;
}

static const size_t VGA_WIDTH=80;
static const size_t VGA_HEIGHT=25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer; //this is the total thing that going to be written in 0xB8000?

void terminal_initialize(void){
    terminal_row=0;
    terminal_column=0;
    terminal_color=vga_entry_color(vga_light_grey,vga_black);
    terminal_buffer=(uint16_t*) 0xb8000;
    for(size_t y=0;y<VGA_HEIGHT;y++){
        for(size_t x=0;x<VGA_WIDTH;x++){
            const size_t index=y*VGA_WIDTH+x;
            terminal_buffer[index]=vga_entry(' ',terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color){
    terminal_color=color;
}

void terminal_putentryat(char c,uint8_t color,size_t x,size_t y){
    const size_t index=y*VGA_WIDTH+x;
    terminal_buffer[index]=vga_entry(c,color); //this is the total thing thats written in bx8000?
}

void terminal_putchar(char c){
    terminal_putentryat(c,terminal_color,terminal_column,terminal_row);
    if(++terminal_column==VGA_WIDTH){
        terminal_column=0;
        if(++terminal_row==VGA_HEIGHT){
            terminal_row=0;
        }
    }
}

void terminal_write(const char* data,size_t size){
    for(size_t i=0;i<size;i++){
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data){
    terminal_write(data,strlen(data));
}

void kernel_main(void){
    terminal_initialize();
    terminal_writestring("unown kernel v0.0.1\n");
    terminal_writestring("Welcome to unownOS\n");
}
