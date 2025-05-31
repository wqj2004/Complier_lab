.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
_spill_0: .word 0
_spill_1: .word 0
_spill_2: .word 0
_spill_3: .word 0
_spill_4: .word 0
_spill_5: .word 0
_spill_6: .word 0
_spill_7: .word 0
_spill_8: .word 0
_spill_9: .word 0
_spill_10: .word 0
_spill_11: .word 0
_spill_12: .word 0
_spill_13: .word 0
_spill_14: .word 0
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

gcd1:
  move $t0, $a0
  move $t1, $a0
  move $t2, $a1
  div $t1, $t2
  mflo $t3
  move $t4, $a1
  mul $t5, $t3, $t4
  sub $t6, $t0, $t5
  move $t7, $t6
label0:
  move $s0, $t7
  li $s1, 0
  bne $s0, $s1, label1
  j label2
label1:
  move $s2, $a1
  move $a0, $s2
  move $s3, $t7
  move $a1, $s3
  move $s4, $a0
  move $s5, $a0
  move $s6, $a1
  div $s5, $s6
  mflo $s7
  move $t8, $a1
  mul $t9, $s7, $t8
  sw $t0, _spill_0
  sub $t0, $s4, $t9
  move $t7, $t0
  j label0
label2:
  sw $t1, _spill_1
  move $t1, $a1
  sw $t2, _spill_2
  move $t2, $t1
  sw $t3, _spill_3
  move $t3, $t2
  move $v0, $t3
  jr $ra

gcd2:
  li $t0, 0
  move $t1, $a0
  move $t2, $a1
  bgt $t1, $t2, label3
  j label4
label3:
  move $t3, $a0
  move $t4, $a0
  move $t5, $a1
  div $t4, $t5
  mflo $t6
  move $t7, $a1
  mul $s0, $t6, $t7
  sub $s1, $t3, $s0
  move $t0, $s1
  move $s2, $t0
  li $s3, 0
  beq $s2, $s3, label6
  j label7
label6:
  move $s4, $a1
  move $v0, $s4
  jr $ra
  j label8
label7:
  move $s5, $a1
  move $s6, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -72
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $t8, 64($sp)
  sw $t9, 68($sp)
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  move $a0, $s5
  move $a1, $s6
  jal gcd2
  addi $sp, $sp, 0
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  addi $sp, $sp, 8
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $t8, 64($sp)
  lw $t9, 68($sp)
  addi $sp, $sp, 72
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s7, $v0
  move $v0, $s7
  jr $ra
label8:
  j label5
label4:
  move $t8, $a1
  move $t9, $a1
  sw $t1, _spill_4
  move $t1, $a0
  sw $t2, _spill_5
  div $t9, $t1
  mflo $t2
  sw $t3, _spill_6
  move $t3, $a0
  sw $t4, _spill_7
  mul $t4, $t2, $t3
  sw $t5, _spill_8
  sub $t5, $t8, $t4
  move $t0, $t5
  sw $t6, _spill_9
  move $t6, $t0
  sw $t7, _spill_10
  li $t7, 0
  beq $t6, $t7, label9
  j label10
label9:
  sw $s0, _spill_11
  move $s0, $a0
  move $v0, $s0
  jr $ra
  j label11
label10:
  sw $s1, _spill_12
  move $s1, $a0
  sw $s2, _spill_13
  move $s2, $t0
  sw $s3, _spill_14
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -72
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $t8, 64($sp)
  sw $t9, 68($sp)
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  move $a0, $s1
  move $a1, $s2
  jal gcd2
  addi $sp, $sp, 0
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  addi $sp, $sp, 8
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $t8, 64($sp)
  lw $t9, 68($sp)
  addi $sp, $sp, 72
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s3, $v0
  move $v0, $s3
  jr $ra
label11:
label5:

main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $v0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  move $t2, $t0
  move $t3, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -72
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $t8, 64($sp)
  sw $t9, 68($sp)
  move $a0, $t2
  move $a1, $t3
  jal gcd1
  addi $sp, $sp, 0
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $t8, 64($sp)
  lw $t9, 68($sp)
  addi $sp, $sp, 72
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t4, $v0
  move $a0, $t4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t5, $t0
  move $t6, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -72
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $t8, 64($sp)
  sw $t9, 68($sp)
  move $a0, $t5
  move $a1, $t6
  jal gcd2
  addi $sp, $sp, 0
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $t8, 64($sp)
  lw $t9, 68($sp)
  addi $sp, $sp, 72
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t7, $v0
  move $a0, $t7
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $s0, 0
  move $v0, $s0
  jr $ra
