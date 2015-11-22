.text
lh $t1, x
lh $t2, y
lh $t3, z
add $t3,$t2,$t1
la $t4,z
sh $t2, -2($t4)
sh $t1, -4($t4)


.data
x: .half 15
y: .half 8
z: .half 0
