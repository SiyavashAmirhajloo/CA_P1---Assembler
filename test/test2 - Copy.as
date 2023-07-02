	addi  2,0,3
	addi  3,2,5
	addi  4,3,-4
	sw	2,2,Fill_2
	lw	5,4,Fill_1
label	add	5,5,2
	slt	6,3,5
	beq	6,0,label
	sub 7,4,2
	beq	6,7,END
END	halt
Fill_1	.fill	-5
Fill_2	.fill   END