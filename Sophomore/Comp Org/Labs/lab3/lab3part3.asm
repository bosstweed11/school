#invert entered string with no spaces

#hello world -> ollehdlrow

# detect space .. stop! reverse that word in specified register, keep track of length of word
#use loop, look at class example
.data 
	
	str_1: .space 64
	str_2: .space 64
	a: .asciiz "Please enter your string: \n"
	c: .asciiz "Here is the output:  "
.text
la $a0,a
li $v0,4
syscall


li $v0, 8
la $a0, str_1
li $a1,64
syscall
move $t0,$a0
la $t2, str_2





loop:
	addi $t4,$t0,0
	
loop2:
	lb $t1,0($t0)
	beq $t1,$0,end
	beq $t1,0x0a,space
	beq $t1,0x20,space
	addi $t0,$t0,1
	
	j loop2
loop3:
	addi $t2,$t2,1
	j loop
	
space:
	
	addi $t3,$t0,-1
	addi $t0,$t0,1
	
	
flip:
	lb $t6,0($t3)
	sb $t6, 0($t2)
	addi $t2,$t2,1
	addi $t3,$t3,-1
	beq $t6,0x0a,end
	lb $t6,0($t3)
	sb $t6, 0($t2)
	beq $t3,$t4,loop3
	
	j flip



end:
	la $a0,c
	li $v0,4
	syscall
	la $a0,str_2
	li $v0,4
	syscall
	
	li $v0, 10
	syscall
	
