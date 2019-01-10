.global main
.global f1
.data


.text
main:
cmp r0,r0
beq f1
mov r5, #1

f1: mov r1,r5
fin:    swi 0x123456
