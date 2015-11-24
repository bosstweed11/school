.data
	str: .asciiz "enter int: "
	str_1: .asciiz "x>5"
	str_2: .asciiz "x<5"
	
.text

li $v0, 4
la $a0, str
syscall

li $v0, 5
syscall

move $t0,$v0

addi $t0,$t0,-5
beq $t0,$zero,EQUAL

li $v0, 4
la $a0, str_2
syscall
j END


EQUAL:
	li $v0,4
	la $a0, str_1
	syscall

END: 
li $v0, 10
syscall



