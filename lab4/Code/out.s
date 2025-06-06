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

hanoi:
  move $t0, $a0
  li $t1, 1
  beq $t0, $t1, label0
  j label1
  li $t3, 0
  bne $t2, $t3, label0
  j label1
label0:
  move $t4, $a1
  li $t5, 1000000
  mul $t6, $t4, $t5
  move $t7, $a3
  add $s0, $t6, $t7
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  sw $ra, 4($sp)
  move $a0, $s0
  jal write
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  j label2
label1:
  move $s1, $a0
  li $s2, 1
  sub $s3, $s1, $s2
  move $s4, $a1
  move $s5, $a3
  move $s6, $a2
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
  addi $sp, $sp, -16
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $s6
  jal hanoi
  addi $sp, $sp, 0
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  addi $sp, $sp, 16
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
  move $t8, $a1
  li $t9, 1000000
  mul $t3, $t8, $t9
  sw $t0, _spill_0
  move $t0, $a3
  sw $t1, _spill_1
  add $t1, $t3, $t0
  addi $sp, $sp, -8
  sw $a0, 0($sp)
  sw $ra, 4($sp)
  move $a0, $t1
  jal write
  lw $a0, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 8
  sw $t2, _spill_2
  move $t2, $a0
  sw $t4, _spill_3
  li $t4, 1
  sw $t5, _spill_4
  sub $t5, $t2, $t4
  sw $t6, _spill_5
  move $t6, $a2
  sw $t7, _spill_6
  move $t7, $a1
  sw $s0, _spill_7
  move $s0, $a3
  sw $s1, _spill_8
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
  addi $sp, $sp, -16
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  sw $a3, 12($sp)
  move $a0, $s0
  jal hanoi
  addi $sp, $sp, 0
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  lw $a3, 12($sp)
  addi $sp, $sp, 16
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
  move $s1, $v0
label2:
  sw $s2, _spill_9
  li $s2, 0
  move $v0, $s2
  jr $ra

main:
  li $t0, 3
  move $t1, $t0
  li $t2, 1
  li $t3, 2
  li $t4, 3
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
  move $a0, $t4
  jal hanoi
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
  move $t5, $v0
  li $t6, 0
  move $v0, $t6
  jr $ra
