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
	.file	"file.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #32
	mov	r3, #2
	str	r3, [fp, #-28]
	mov	r3, #32
	str	r3, [fp, #-8]
	mov	r3, #116
	strb	r3, [fp, #-9]
	mov	r3, #99
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-20]
	ldr	r0, [fp, #-16]
	ldr	r1, [fp, #-20]
	bl	foo_0
	ldrb	r3, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r3
	ldr	r1, [fp, #-8]
	bl	foo_1
	sub	r3, fp, #32
	mov	r0, r3
	bl	foo_3
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	main, .-main
	.ident	"GCC: (GNU) 4.5.3"
