.data
A:	.space	64
B:	.space	64
C:	.space	64

A_str: 	.asciiz	"Please enter the first 16-bit binary number: "
B_str:	.asciiz	"Please enter the second 16-bit binary number: "
C_str:	.asciiz	"Sum is: "
OV_str:	.asciiz	"\nOverflow bit: "

.text
	j main		# DO NOT EDIT THIS LINE

########################
	
	la $a0, A_str
	la $a1,B_str
	la $a2,C_str
	li $t1,0
	jal AddNumbers

########################
# BitAdder
#	adds two bits with the carry in and outputs the 1-bit sum and carry out for the next step
# INPUT:
# 	BitAdder expects arguments in $a0, $a1, $a2
# 	$a0 = specific bit (of values either 0 or 1 in decimal) from A, do not pass character '0' or '1'
# 	$a1 = specific bit (of values either 0 or 1 in decimal) from B, do not pass character '0' or '1'
# 	$a2 = carry in (of values either 0 or 1 in decimal) from previous step
# OUTPUT: 
# 	$v0 = 1-bit sum in $v0 
#	$v1 = carry out for the next stage
BitAdder:
	addi    $sp,$sp,-4        # push space for activation frame
   	sw    $ra,0($sp)       	 # save return address
   	bnez $a2, carryin		# if theres a carry in, account for it, else dont
   	beq  $a1,$a0,compare	# if no carry in, compare two bits to see if there will be a carry out
   	add $v0,$a0,$a1			# if they are not equal, and no carry they will add to 1
   	li $v1,0				# since if they are not equal and there is no carry in, there will be no carry out
   	j end
compare:
	beq $a1,1,overflow
	add $v0,$a0,$a1					#if no carry in, and $a1 (which is equal to $a0) is not one, $v0 will be 0 and there will be no carry out
	li $v1,0
	j end
compare2:
	beq $a1,1,overflow2
	add $v0,$a0,$a2					# if carry in, and both digits are 0's then v0 = 1, v1 = 0
	li $v1,0
	j end
overflow:
	li $v0,0					#if both bits are 1 with no carry in, there will be a sum of 0 and a carry out of 1
	li $v1,1
	j end
overflow2:
	li $v0,1					#if carry in of 1 and both bits are 1s, both v0 and v1 are 1
	li $v1,1
	j end
carryin:
	beq $a1,$a0,compare2			
	j overflow				#since one bit is 0 and the other is 1, with a carry in the sum is 0 with a carry of one, we have this condition so just branch to it

end:	
	lw $ra,0($sp)
	addi $sp,$sp,4

	# return
        jr $ra


# AddNumbers 
# 	it adds two strings, each of which represents a 8-bit number 
# INPUT:
# 	$a0 = address of A
# 	$a1 = address of B
# 	$a2 = address of C
# OUTPUT:
#	$v0 = overflow bit (either 0 or 1 in decimal)
AddNumbers:
	addi   $sp,$sp,-4        # push space for activation frame
   	sw   $ra,0($sp)       	 # save return address
	li $t2,0
	move $t4,$a0
   	move $t5,$a1
   	move $t6,$a2
   	addi $t5,$t5,15
   	addi $t4,$t4,15
   	addi $t6,$t6,15
addnum:
	lb $a0,($t4)
	lb $a1,($t5)
	subi $a0, $a0, 0x30
	subi $a1,$a1,0x30
	bne $t2,0,next				#else no carry in
	
nocarry:
	li $a2,0
next:
	
	# loop 16 times for 16 bits 
	jal BitAdder
	
	move $a2,$v1
	addi $v0,$v0,0x30
	sb $v0,($t6)
	addi $t2,$t2,1
	subi $t4,$t4,1
	subi $t5,$t5,1
	subi $t6,$t6,1
	bne $t2,16,addnum
	# loop ends, set $v0 with the overflow
	li $v0,0
	beq $v1,1,setoverflow
	li $v0, 0
	
	j epilogue
setoverflow:
	li $v0,1
	
epilogue:
	lw $ra,0($sp)
	addi $sp,$sp,4
	# epilogue

	
	# return
	jr $ra


#============================================== 
#Do NOT edit the rest of the code in this file.
#============================================== 

main: #
        jal setRegisterStates

	# print A_str
	la	$a0, A_str
	li	$v0, 4
	syscall

	# read A
	la	$a0, A
	li	$a1, 64
	li	$v0, 8
	syscall

	# print B_str
	la	$a0, B_str
	li	$v0, 4
	syscall

	# read B
	la	$a0, B
	li	$a1, 64
	li	$v0, 8
	syscall

	# clip A and B to 16-characters
	li	$t0, 0x00
	la	$t1, A
	sh	$t0, 16($t1)
	la	$t1, B
	sh	$t0, 16($t1)

	# call AddNumbers
	la	$a0, A
	la	$a1, B
	la	$a2, C
        jal AddNumbers
	
	move	$t3, $v0	# save overflow bit

	# clip C to 16-characters
	li	$t0, 0x00
	la	$t1, C
	sh	$t0, 16($t1)

	# print C_str
	la	$a0, C_str
	li	$v0, 4
	syscall

	# print C
	la	$a0, C
	li	$v0, 4
	syscall

	# print OV_str
	la	$a0, OV_str
	li	$v0, 4
	syscall

	# print overflow
	move	$a0, $t3
	li	$v0, 1
	syscall
	
	# done
        jal checkRegisterStates

        li $v0, 10          #Exit
        syscall

setRegisterStates:
    li $s0, -1
    li $s1, -1
    li $s2, -1
    li $s3, -1
    li $s4, -1
    li $s5, -1
    li $s6, -1
    li $s7, -1
    sw $sp, old_sp_value
    sw $s0, ($sp)       #Write something at the top of the stack
    jr $ra

checkRegisterStates:

    bne $s0, -1, checkRegisterStates_failedCheck
    bne $s1, -1, checkRegisterStates_failedCheck
    bne $s2, -1, checkRegisterStates_failedCheck
    bne $s3, -1, checkRegisterStates_failedCheck
    bne $s4, -1, checkRegisterStates_failedCheck
    bne $s5, -1, checkRegisterStates_failedCheck
    bne $s6, -1, checkRegisterStates_failedCheck
    bne $s7, -1, checkRegisterStates_failedCheck

    lw $t0, old_sp_value
    bne $sp, $t0, checkRegisterStates_failedCheck

    lw $t0, ($sp)
    bne $t0, -1, checkRegisterStates_failedCheck

    jr $ra                      #Return: all registers passed the check.
    
    checkRegisterStates_failedCheck:
        la $a0, failed_check    #Print out the failed register state message.
        li $v0, 4
        syscall

        li $v0, 10              #Exit prematurely.
        syscall

.data
	old_sp_value:   .word 0
	failed_check:   .asciiz "One or more registers was corrupted by your code.\n"
	
	
