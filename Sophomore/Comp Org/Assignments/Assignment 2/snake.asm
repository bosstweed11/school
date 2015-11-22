#Written by Andrew Beers 3/27/11
#COE 0147 ASSIGNMENT 2: SNAKE



.data

endgame_:	.asciiz		"\nGame over.\n"
gametime:	.asciiz		"The playing time was "
ms:			.asciiz		" ms.\n"
gamescore:	.asciiz		"Your game score was "
points:		.asciiz		" frogs.\n"
point:		.asciiz		" frog. \n"

green_x:		.word 		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
green_y:		.word		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
green_color: .word		3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3

#wall_x:		.word		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
#wall_y:		.word		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1

head_x: 		.word		7
head_y:		.word		31
body_x:	.word		1,2,3,4,5,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
body_y:	.word		31,31,31,31,31,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
tail_x:		.word		0
tail_y:		.word		31
snake_color:	.word		2

#### Initialization
.text
	li $v0, 30				#start timer
	syscall
	move $s2,$a0			#save time value
	
	li $s1,6					#load number of body leds		
	move $s0,$s1	
	li $k1,0					#initialze pressed button to 0
	jal calcLED				# call to get 32 random green frogs
	jal setGreen				#call to display all frogs
	
	#attempt at extra credit 1
	#jal setWalls
			
	lw $a0,head_x			#load initial head of snake and plot it
	lw $a1,head_y			
	lw $a2,snake_color
	jal setLED
	
	jal setBody				# set the body of the snake
	
	lw $a0, tail_x			#set the tail of the snake
	lw $a1,tail_y
	lw $a2,snake_color
	
	jal setLED
	
#main loop of the program	
_poll:
	#jal setWalls
	move $s0,$s1			#update length of the snake
	beq $k1,32,endgame		#if all frogs caught, end game
	li $t0, 0xFFFF0000
	lw $t1,0($t0)
	and $t1,$t1,1
	bne $t1,$zero,_keypress		# identify if a key has been pressed
poll:
	jal timer						#emperically time 200 ms
	beq $k0,1,_keypress			#go to current direction
	beq $k0,2,_keypress
	beq $k0,3,_keypress
	beq $k0,4,_keypress

	
	j _poll
	
	

	
## shuts off tail if last was not a frog
_keypress:	
	
	
	lw $a0,tail_x		#load tail coordinates
	lw $a1,tail_y
	li $a2,2				#load yellow for tail color
	beq $s3,1,skipz		#if the head ran over a frog last move, keep tail yellow
	li $a2,0				#else turn it off (black)
skipz:

	jal setLED				# light tail
	li $t0, 0xFFFF0004
	lw $t1, ($t0)			
	## figure out which button was pushed, and act accordingly
	beq $t1, 224, up_key
	beq $t1, 225, down_key
	beq $t1, 226, left_key
	beq $t1, 227, right_key
	
	j _poll
	

# all key functions are below
up_key:
	beq $k0,2,down_key		# if current direction is down, and player tries to go up, override and continue going down
	beq $s3,1,back_up		#if the snake ate a frog last time, skip moving and just add an led
	jal moveSnake			# else move the snake in the appropriate direction
	j up
back_up:
	jal addLED				#insert new body led
up:
	lw $t0,head_y			# load the y term
	subi $t0,$t0,1			# subtract one for directoin
	blt $t0,$zero,cross_top	# check if led has gone over the top
	j no_cross_top
cross_top:
	li $t0,64					# if over the top display on bottom
no_cross_top:
	sw $t0, head_y			# store value in the head y value			
	li $k0,1					#assign current direction
	j key_done

down_key:	
	beq $k0,1,up_key		# if current direction is up, and player tries to go down, override and continue going up
	beq $s3,1,back_down
	jal moveSnake			# else move the snake in the appropriate direction
	j down
back_down:	
	jal addLED				#add body led
down:	
	lw $t0,head_y			#load head y value
	addi $t0,$t0,1
	bge $t0,64,cross_bottom	# see if it crossed over the bottom
	j no_cross_bottom
	
cross_bottom:
	li $t0,0					# if so display on the top
no_cross_bottom:
	sw $t0,head_y			# store head y value
	li $k0,2					#assign current direction
	j key_done

left_key:
	beq $k0,4,right_key		# if current direction is right, and player tries to go left, override and continue going right
	beq $s3,1,back_left		
	jal moveSnake			# else move the snake in the appropriate direction
	j left
back_left:	
	jal addLED				# add body led
left:
	lw $t0,head_x			#load x term
	subi $t0,$t0,1
	blt $t0,$zero,cross_left	#check if it goes out of bounds to the left
	j no_cross_left
	
cross_left:
	li $t0,63					# if so display on right
	
no_cross_left:
	sw $t0,head_x			#store value
	li $k0,3					#assign current direction
	j key_done

right_key:
	beq $k0,3,left_key		# if current direction is left, and player tries to go right, override and continue going left
	beq $s3,1,right_down
	jal moveSnake			# else move the snake in the appropriate direction
	j right
right_down:
	jal addLED				# add body led
right:
		
	lw $t0,head_x			# load x term
	addi $t0,$t0,1
	bge $t0,64,cross_right	# check if it goes off to the right
	j no_cross_right
	
cross_right:
	li $t0,0					# if so display on left
no_cross_right:
	sw $t0,head_x			#store this value
	li $k0,4					#assign current direction
	j key_done	
	
key_done:
	lw $a0,head_x			# load coordinates  to see if the head is on a frog, or itself, or black
	lw $a1,head_y
	lw $a2,snake_color
	jal getLED
	
	jal setLED				# set head value
	#beq $v0,1,change
	beq $v0,2,endgame		# if it is on yellow, its on itself, so end game
	beq $v0,3,found			# if it is on green, its on a frog, increase score and snake length
	
	li $s3,0					# if not found set no frog found
	j _poll
found:
	li $s3,1					# set frog found
	addi $k1,$k1,1			# increment counter

	j _poll

## attempt at extra credit 1
#change:
#	beq $k0,1,_right
#	beq $k0,2,_right
#	beq $k0,3,_up
#	beq $k0,4,_up
#_right:
#	addi $s4,$s4,1
#	li $t1,227
#	beq $s4,2,_left
#	j right_key
#_left:
#	addi $s4,$s4,1
#	li $t1,226
#	beq $s4,4,endgame
#	j left_key
#_up:
#	addi $s4,$s4,1
#	li $t1,224
#	beq $s4,2,_down
#	j up_key
#_down:
#	addi $s4,$s4,1
#	li $t1,225
#	beq $s4,4,endgame
#	j down_key
#	
## given setled function
setLED:
	subi $sp, $sp, 20
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
	# byte offset into display = y * 16 bytes + (x / 4)
	sll	$t0,$a1,4      # y * 16 bytes
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
	
	
## given getled function
getLED:
	subi	$sp, $sp, 20
	sw	$t0, 0($sp)
	sw	$t1, 4($sp)
	sw	$t2, 8($sp)
	sw	$t3, 12($sp)
	sw	$ra, 16($sp)

	jal	_getLED

	lw	$t0, 0($sp)
	lw	$t1, 4($sp)
	lw	$t2, 8($sp)
	lw	$t3, 12($sp)
	lw	$ra, 16($sp)
	addi	$sp, $sp, 20
	
	jr	$ra
_getLED:
	# byte offset into display = y * 16 bytes + (x / 4)
	sll  $t0,$a1,4      # y * 16 bytes
	srl  $t1,$a0,2      # x / 4
	add  $t0,$t0,$t1    # byte offset into display
	la   $t2,0xffff0008
	add  $t0,$t2,$t0    # address of byte with the LED
	# now, compute bit position in the byte and the mask for it
	andi $t1,$a0,0x3    # remainder is bit position in byte
	neg  $t1,$t1        # negate position for subtraction
	addi $t1,$t1,3      # bit positions in reverse order
    	sll  $t1,$t1,1      # led is 2 bits
	# load LED value, get the desired bit in the loaded byte
	lbu  $t2,0($t0)
	srlv $t2,$t2,$t1    # shift LED value to lsb position
	andi $v0,$t2,0x3    # mask off any remaining upper bits
	jr   $ra
	
### calculates all random indicies for the frogs
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
	
	#actual random function, produces 64 random numbers from 0-63 for the x values
random:
	
	li $v0, 42				
	syscall
	sw $a0, ($t1)			# store random number in appropriate address location
	addi $t0,$t0,1			# increment counter	
	addi $t1,$t1,4			#increment address
	beq $t0,32,randomy  	#when done proceed to do the random y values
	beq $t0,64,end
	#extra credit 1
	#beq $t0,76,randomwallsy
	#beq $t0,88,end
	
	j random
## default return function
end:
	lw $ra, ($sp)
	addi $sp,$sp,4
	jr $ra
	
randomy:
	la $t1, green_y		# change address to green y values
	j random
	
#extra credit 1
#randomwalls:
#	la $t1,wall_x
#	j random
#randomwallsy:
#	la $t1,wall_y
#	j random
	
# displays all the frogs
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
	jal setLED

	addi $t3,$t3,1	#increment counter
	addi $t0,$t0,4	#increment to next x value
	addi $t1,$t1,4	#increment to next y value
	addi $t2,$t2,4
	beq $t3,32,end
	
	j _setGreen

#attempt at extra credit 1
#setWalls:
#	subi $sp,$sp,4
#	sw $ra,($sp)
#	li $t3,0			#counter for how many greens have been set
#	la $t0,wall_x	# pointer to green x values
#	la $t1,wall_y	# pointer to green y values
#	li,$a2,1
#_setWalls:
	
#	lw $a0,($t0)		# load values to arguments to set up setLED function
#	lw $a1,($t1)		
#	jal setLED
#	addi $t3,$t3,1	#increment counter
#	addi $t0,$t0,4	#increment to next x value
#	addi $t1,$t1,4	#increment to next y value
#	beq $t3,12,end
	
#	j _setWalls
	
### this function will light up the body of the snake
setBody:
	subi $sp,$sp,4
	sw $ra,($sp)
	
	la $t0,body_x		#load x value pointer
	la $t1,body_y		#load y value pointer
	li $a2,2					# load color of red
	
_setBody:

	
	lw $a0,($t0)				#get x value
	lw $a1,($t1)				#get y value
	
	jal setLED				#plot led
	
	subi $s0,$s0,1			#decrement counter
	addi $t0,$t0,4			# move to next table value
	addi $t1,$t1,4			# move to next table value
	beqz $s0,end
	j _setBody
	
## main moving function of program
moveSnake:
	subi $sp,$sp,4
	sw $ra,($sp)
	
	move $s0,$s1			# get current length

	la $t0,body_x		#load addresses for body and tail
	la $t1,body_y
	la $t2,tail_x
	la $t3,tail_y
	li $a2,2
	move $t4,$t0		#make a copy of the pointers for x and y values
	move $t5,$t1
	
	lw $a0,($t0)			#store the old body as current tail
	lw $a1,($t1)
	sw $a0,($t2)			#set this as the tail x and tail y values
	sw $a1,($t3)
	
	jal setLED
	addi $t0,$t0,4		#increment one of the table values
	addi $t1,$t1,4		
	subi $s0,$s0,1		#subtract from body size
_moveSnake:
	lw $a0,($t0)			#load next value for body, x and y
	lw $a1,($t1)
	sw $a0,($t4)			#store this value in one spot behind of where it is now
	sw $a1,($t5)		
	jal setLED
	addi $t0,$t0,4		#increment all pointers
	addi $t4,$t4,4
	addi $t1,$t1,4
	addi $t5,$t5,4
	subi $s0,$s0,1
	beqz $s0,preend

	j _moveSnake
	
preend:
	lw $a0,head_x		#load current head value
	lw $a1,head_y
	sw $a0,($t4)			#set this as the first body value
	sw $a1,($t5)

	j end	
	
### this function times 200 ms then continues moving the snake in the current direction
timer:
	subi $sp,$sp,4
	sw $ra,($sp)
	li $v0,30		#call timing function
	syscall
	move $t1,$a0
_timer:
	
	syscall
	sub $t2,$a0,$t1		#keep subtracting type from initial time until the difference is 200 ms
	bge $t2,200,end
	
	j _timer
## function to add a body value to the snake	
addLED:
	subi $sp,$sp,4
	sw $ra,($sp)
	move $t4,$s0
	sll $t4,$t4,2			# calculate offset for table of body values
	lw $t0,head_x		#load head
	lw $t1,head_y		
	la $t2,body_x	#load body
	la $t3,body_y
	
	add $t2,$t2,$t4		# get offset for body
	add $t3,$t3,$t4
	
	sw $t0,($t2)			#store old head in the new body place
	sw $t1,($t3)
	
	addi $s1,$s1,1		#add to overall body length
	
	j end
endgame:
	la $a0,endgame_		#end game gracefully
	li $v0,4
	syscall
	
	li $v0, 30
	syscall
	sub $a1,$a0,$s2			# calculate total game time
	
	la $a0, gametime		# display string
	li $v0,4
	syscall
	move $a0,$a1
	li $v0,1
	syscall					#show game time
	la $a0,ms
	li $v0,4
	syscall
	la $a0, gamescore		# display game score string
	li $v0,4
	syscall
	move $a0,$k1
	li $v0,1
	syscall					#display score
	beq $k1,1,p				#if score 1, say frog, else say frogs
	la $a0,points			
	li $v0,4
	syscall
	li $v0,10
	syscall				#end program
p:
	la $a0,point
	li $v0,4
	syscall
	li $v0,10
	syscall				#end program
	
	
	
