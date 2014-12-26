#ifndef IDT_H
#define IDT_H	
#define SUP_CODESEL 0x8
typedef struct
{
   USHORT lowoffset;
   USHORT selector;
   UCHAR params;
   USHORT access;
   UCHAR hioffset;
} __attribute__ ((packed)) idtdisc;

typedef struct 
{
 USHORT limit;
 ULONG base;
} __attribute__ ((packed)) discreg;

void loadidt(discreg *idtptr);
void isr0();
void isr1();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr16();
void isr17();
void isr18();
void isr19();
void int0();
void int1();
void n();
void lowisr();
void isr31();
void intelreserved();

discreg idt;

int addisr(unsigned long num, void (*function) (void))
{
idtdisc* temp;
	temp = (idtdisc *) (idt.base + (num * 8));
	temp->lowoffset = (USHORT) (((ULONG) function & 0xffff));
	temp->selector = (USHORT) SUP_CODESEL;
	temp->access = (UCHAR) 0x8e;
	temp->params = (UCHAR) 0;
	temp->hioffset = (USHORT) (((ULONG) function >> 16));
}  

int setupidt()
{
idt.limit=0x2f;
addisr(0x0,isr0);
addisr(0x1,isr1);
addisr(0x2,intelreserved);
addisr(0x3,isr3);
addisr(0x4,isr4);
addisr(0x5,isr5);
addisr(0x6,isr6);
addisr(0x7,isr7);
addisr(0x8,isr8);
addisr(0x9,isr9);
addisr(0xa,isr10);
addisr(0xb,isr11);
addisr(0xc,isr12);
addisr(0xd,isr13);
addisr(0xe,isr14);
addisr(0xf,intelreserved);
addisr(0x10,isr16);
addisr(0x11,isr17);
addisr(0x12,isr18);
addisr(0x13,isr19);
addisr(0x14,n);
addisr(0x15,n);
addisr(0x16,n);
addisr(0x17,n);
addisr(0x18,n);
addisr(0x19,n);
addisr(0x1a,n);
addisr(0x1b,n);
addisr(0x1c,n);
addisr(0x1d,n);
addisr(0x1e,n);
addisr(0x1f,n);
addisr(0x20,int0);
addisr(0x21,int1);
addisr(0x22,lowisr);
addisr(0x23,lowisr);
addisr(0x24,lowisr);
addisr(0x25,lowisr);
addisr(0x26,lowisr);
addisr(0x27,lowisr);
addisr(0x28,isr31);
addisr(0x29,isr31);
addisr(0x2a,isr31);
addisr(0x2b,isr31);
addisr(0x2c,isr31);
addisr(0x2d,isr31);
addisr(0x2e,isr31);
addisr(0x2f,isr31);

}


#endif
