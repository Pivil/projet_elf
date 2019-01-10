.data
D1: .byte 12

.text
.global main
main:
LDR r3, ptrD1
LDR r4, [r3]
fin:    swi 0x123456

ptrD1 : .hword D1
