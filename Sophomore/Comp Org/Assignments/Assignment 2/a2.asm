.data
R_str:		.asciiz		"RIGHT key was pressed\n"
L_str:		.asciiz		"LEFT key was pressed\n"
U_str:		.asciiz		"UP key was pressed\n"
D_str:		.asciiz		"DOWN key was pressed\n"
C_str:		.asciiz		"CENTER key was pressed\n"


endgame_:	.asciiz		"Game over."


green_x:		.word 		-1,-1,-1,-1,-1
green_y:		.word		-1,-1,-1,-1,-1
green_color: .word		3,3,3,3,3

red_x: 		.word		64
red_y:		.word		3
red_color:	.word		1

.text
	li $k0,0
	jal calcLED
	jal setGreen

	lw $a0,red_x
	lw $a1,red_y
	lw $a2,red_color
	jal setLED
	
_poll:
	
	beq $k0,5,endgame
	li $t0, 0xFFFF0000
	lw $t1,0($t0)
	and $t1,$t1,1
	bne $t1,$zero,_keypress
	
	j _poll
	
_keypress:
	lw $a0,red_x
	lw $a1,red_y
	li $a2,0
	jal setLED

	li $t0, 0xFFFF0004
	lw $t1, ($t0)
	
	beq $t1, 66, center_key
	beq $t1, 224, up_key
	beq $t1, 225, down_key
	beq $t1, 226, left_key
	beq $t1, 227, right_key
	
	j _poll
	
center_key:
	
	beq $a3,1,killgreen
_center:
	la $a0, C_str	
	j key_done

up_key:
	lw $t0,red_y
	subi $t0,$t0,1
	blt $t0,$zero,cross_top
	j no_cross_top
	
cross_top:
	li $t0,63
	
no_cross_top:
	sw $t0, red_y
	la $a0, U_str
	
	j key_done

down_key:
	lw $t0,red_y
	addi $t0,$t0,1
	bge $t0,64,cross_bottom
	j no_cross_bottom
	
cross_bottom:
	li $t0,0
no_cross_bottom:
	sw $t0,red_y
	
	la $a0, D_str
	j key_done

left_key:
	lw $t0,red_x
	subi $t0,$t0,1
	blt $t0,$zero,cross_left
	j no_cross_left
	
cross_left:
	li $t0,63
	
no_cross_left:
	sw $t0,red_x
	la $a0, L_str
	j key_done

right_key:
	lw $t0,red_x
	addi $t0,$t0,1
	bge $t0,64,cross_right
	j no_cross_right
	
cross_right:
	li $t0,0
no_cross_right:
	sw $t0,red_x
	la $a0, R_str
	j key_done	
	
key_done:
	
	li $v0,4
	syscall
	
	
	
	jal setGreen
	
	
	lw $a0,red_x
	lw $a1,red_y
	lw $a2,red_color
	jal setLED
	
	jal checkGreen
	
	j _poll

setLED:
	subi	$sp, $sp, 20
	sw	$t0, 0($sp)
	sw	$t1, 4($sp)
	sw	$t2, 8($sp)
	sw	$t3, 12($sp)
	sw	$ra, 16($sp)

	jal	_setLED

	lw	$t0, 0($sp)
	lw	$t1, 4($sp)
	lw	$t2, 8($sp)
	lw	$t3, 12($sp)
	lw	$ra, 16($sp)
	addi	$sp, $sp, 20
	
	jr	$ra


	# void _setLED(int x, int y, int color)
	#   sets the LED at (x,y) to color
	#   color: 0=off, 1=red, 2=orange, 3=green
	#
	# arguments: $a0 is x, $a1 is y, $a2 is color
	# trashes:   $t0-$t3
	# returns:   none
	#
_setLED:
	# byte offset into display = y * 32 bytes + (x / 4)
	sll	$t0,$a1,5      # y * 32 bytes
	srl	$t1,$a0,2      # x / 4
	add	$t0,$t0,$t1    # byte offset into display
	li	$t2,0xffff0008	# base address of LED display
	add	$t0,$t2,$t0    # address of byte with the LED
	# now, compute led position in the byte and the mask for it
	andi	$t1,$a0,0x3    # remainder is led position in byte
	neg	$t1,$t1        # negate position for subtraction
	addi	$t1,$t1,3      # bit positions in reverse order
	sll	$t1,$t1,1      # led is 2 bits
	# compute two masks: one to clear field, one to set new color
	li	$t2,3		
	sllv	$t2,$t2,$t1
	not	$t2,$t2        # bit mask for clearing current color
	sllv	$t1,$a2,$t1    # bit mask for setting color
	# get current LED value, set the new field, store it back to LED
	lbu	$t3,0($t0)     # read current LED value	
	and	$t3,$t3,$t2    # clear the field for the color
	or	$t3,$t3,$t1    # set color field
	sb	$t3,0($t0)     # update display
	jr	$ra
	
calcLED:
	subi $sp,$sp,4
	sw $ra,($sp)
	
	li $t0,0			#initialize counter to 0
	la $t1,green_x	#initialize location of green x lookup table in $t1
	li $v0,30		#get system clock
	syscall
	move $a1,$a0	#seed random number generator
	li $a0,1
	li $v0,40	
	syscall
	lw $t2,($t1)	
	li $a1,63
	
	#actual random function, produces 5 random numbers from 0-127 for the x values
random:
	li $v0, 42				
	syscall
	sw $a0, ($t1)
	
	addi $t0,$t0,1
	addi $t1,$t1,4
	beq $t0,5,randomy  #when done proceed to do the random y values
	beq $t0,10,end
	j random
end:
	lw $ra, ($sp)
	addi $sp,$sp,4
	jr $ra
	
randomy:
	li $a1,63			#change random high value to 7 then redo random values
	la $t1, green_y
	j random
	
setGreen:
	subi $sp,$sp,4
	sw $ra,($sp)
	
	li $t3,0			#counter for how many greens have been set
	la $t0,green_x	# pointer to green x values
	la $t1,green_y	# pointer to green y values
	la $t2,green_color
_setGreen:
	
	lw $a0,($t0)		# load values to arguments to set up setLED function
	lw $a1,($t1)		
	lw $a2,($t2)
	bne $a0,-1,skip	# if this green has been found, make black
	
	sw $zero,($t0)
	sw $zero,($t1)
	sw $zero,($t2)	
	li $a0,0			# make green black
	li $a1,0
	li $a2,0
	
	
skip:
	jal setLED	
	addi $t3,$t3,1	#increment counter
	addi $t0,$t0,4	#increment to next x value
	addi $t1,$t1,4	#increment to next y value
	addi $t2,$t2,4
	beq $t3,5,end
	
	j _setGreen
	
checkGreen:
	li $a3,0
	li $t0,0
	la $t1,green_x
	la $t2,green_y
	li $t4,0
	lw $s1,red_x			#load red x value
	lw $s2,red_y			#load red y value
_checkGreen:
	lw $t3,($t1)				# get green  x value
	move $a0,$t3
	li $v0,1					# print out green x value
	syscall
	
	beq $a0,$s1,checky		# if the current red value matches a green x value, try the corresponding y value
	addi $t0,$t0,1			# else increment counter
	addi $t1,$t1,4			# increment $t1 displacement
	addi $t4,$t4,4			# keep track of how much $t1 moves to add on to the y table later to make sure you match corresponding y values
	beq $t0,5,_poll			# if 5th time and no match poll again
	
	j _checkGreen
checky:
	
	add $t2,$t2,$t4			# add offset to y look up table
	lw $t3,($t2)				# get corresponding green y value
	
	move $a0,$t3			# print the green y value
	li $v0,1	
	syscall
	
	beq $s2,$t3,match		# if the green y matches red y, red is on green
	
	j _poll					# else poll again
match:
	li $a3,1					#on flag for red on top of green
	j _poll
killgreen:
	subi $sp,$sp,4
	sw $ra,($sp)
	
	li $t0,0
	li $t2,0
	lw $a0,red_x			#load red x value			
	lw $a1,red_y			#load red y value	
	la $s1,green_x			#load green x value
	la $s2,green_y			# load green y value
	la $s3,green_color
_killgreen:	
	
	lw $t1,($s1)				# get green value to compare to x value
	beq $a0,	$t1,killgreen_y	# if match swap with -1 and get y coordinate	
	addi $t0,$t0,1			#increment counter		
	addi $s1,$s1,4			#increment index of green values
	addi $t2,$t2,4			#increment change in table entry
	addi $s3,$s3,4
	beq $t0, 5, end			# this should never occur, becasue if we are in here, there is a green on below red, however for debugging purposes it is here
	
	j _killgreen
killgreen_y:
	bnez $t1,continue		#check if the coordinates are 0,0
	bnez $k0,_center		# if so, then it is the shadow bit and will not be incremented

continue:
	li $t1,-1					# reassign green value to -1
	sw $t1,($s1)				# store -1 in green value
	
	add $s2,$s2,$t2			#add offset to green y table
	lw $t1,($s2)
	beq $a1,$t1,_killgreen_y	# make sure red y and green y match
	
	j _center
	
_killgreen_y:
	add $s3,$s3,$t2
	lw $s4,($s3)				#check if it has been found
	
	li $t3,-1				# reassign y value to -1
	sw $t3,($s2)			# store value
	addi $k0,$k0,1		#increment counter
	
	j _center
	
endgame:
	la $a0,endgame_		#end game gracefully
	li $v0,4
	syscall
	li $v0,10
	syscall
	
