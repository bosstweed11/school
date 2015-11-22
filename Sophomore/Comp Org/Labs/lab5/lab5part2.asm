# Written by Andrew Beers 2/22/12

.data
	enter: .asciiz "Enter the number of disks: \n"
	exit: .asciiz "The number of moves required is: \n"
	
.text
	
	li $s1,2				#s1 holds the 2 that will be multiplied through
	la $a0,enter			#display prompt
	li $v0,4	
	syscall
	li $v0,5	
	syscall	
	move $s0,$v0		# save entry
	jal prehanoi
premain2:
	beq $t1,1,nosub		
	subi $t1,$t1,1
main2:
	la $a0, exit
	li $v0,4
	syscall
	move $a0,$t1		#load answer, print answer
	li $v0,1
	syscall
	li $v0,10
	syscall
prehanoi:
	li $t1,1				
hanoi:
	addi    $sp,$sp,-8        # push space for activation frame
  	sw    $s0,0($sp)        	# save $s0, which we use
   	sw    $ra,4($sp)       	 # save return address
	beqz $s0,hanoi_exit			
	subi $s0,$s0,1
	jal hanoi
	mult $t1,$s1			#multiply by two
	mflo $t1				#store multiplication
	


hanoi_exit:
	lw    $ra,4($sp)        # restore $ra
   	lw    $s0,0($sp)        # restore $s0
    	addi    $sp,$sp,8        # pop activation frame
    	jr $ra
 nosub:
	j main2
