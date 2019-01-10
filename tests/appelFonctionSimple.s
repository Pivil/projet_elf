.cpu arm7tdmi
.fpu softvfp
.eabi_attribute 20, 1
.eabi_attribute 21, 1
.eabi_attribute 23, 3
.eabi_attribute 24, 1
.eabi_attribute 25, 1
.eabi_attribute 26, 1
.eabi_attribute 30, 6
.eabi_attribute 18, 4
.file	"example5.c"
.text
.align	2
.global	funcA
.type	funcA, %function
funcA:
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 1, uses_anonymous_args = 0
@ link register save eliminated.
str	fp, [sp, #-4]!
add	fp, sp, #0
mov	r3, #1
mov	r0, r3
add	sp, fp, #0
ldmfd	sp!, {fp}
bx	lr
.size	funcA, .-funcA
.align	2
.global	main
.type	main, %function
main:
@ args = 0, pretend = 0, frame = 8
@ frame_needed = 1, uses_anonymous_args = 0
stmfd	sp!, {fp, lr}
add	fp, sp, #4
sub	sp, sp, #8
bl	funcA
mov	r3, r0
strb	r3, [fp, #-5]
mov	r3, #0
mov	r0, r3
sub	sp, fp, #4
ldmfd	sp!, {fp, pc}
.size	main, .-main
.ident	"GCC: (GNU) 4.5.3"
