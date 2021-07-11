main:
    addi $a0, $zero, 19654 # a0= 19654
    addi $a1, $zero, 100654  # a1=100654
    addi $a2, $zero, 10065  # a2= 10065
    add $a0,$a0,$a1
    add $a0,$a0,$a2 #a0琌砆埃计
    addi $a1,$zero,3  #a1琌埃计
  
   
    addi $v0 $zero 0                 #$v0纗坝
    jal average  # call average
    j exit
average:

    addi $sp, $sp, -4
    sw $ra, 0x0($sp) 
    

    slt $t1, $a1,$a0
    bne $t1,1,breakk
    addi $v0,$v0,1 
     
    addi $a0, $a0 -3          #緇计(砆埃计)=緇计(砆埃计)-埃计
    
    
    j average
breakk:
    
     lw $ra, 0x0($sp)    # Load $ra register from stack
    addi $sp, $sp, 4
    jr $ra
   

exit:
    