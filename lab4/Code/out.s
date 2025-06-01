.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
a: .space 200
b: .space 200
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
_spill_15: .word 0
_spill_16: .word 0
_spill_17: .word 0
_spill_18: .word 0
_spill_19: .word 0
_spill_20: .word 0
_spill_21: .word 0
_spill_22: .word 0
_spill_23: .word 0
_spill_24: .word 0
_spill_25: .word 0
_spill_26: .word 0
_spill_27: .word 0
_spill_28: .word 0
_spill_29: .word 0
_spill_30: .word 0
_spill_31: .word 0
_spill_32: .word 0
_spill_33: .word 0
_spill_34: .word 0
_spill_35: .word 0
_spill_36: .word 0
_spill_37: .word 0
_spill_38: .word 0
_spill_39: .word 0
_spill_40: .word 0
_spill_41: .word 0
_spill_42: .word 0
_spill_43: .word 0
_spill_44: .word 0
_spill_45: .word 0
_spill_46: .word 0
_spill_47: .word 0
_spill_48: .word 0
_spill_49: .word 0
_spill_50: .word 0
_spill_51: .word 0
_spill_52: .word 0
_spill_53: .word 0
_spill_54: .word 0
_spill_55: .word 0
_spill_56: .word 0
_spill_57: .word 0
_spill_58: .word 0
_spill_59: .word 0
_spill_60: .word 0
_spill_61: .word 0
_spill_62: .word 0
_spill_63: .word 0
_spill_64: .word 0
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

mod:
  move $t0, $a0
  move $t1, $a0
  move $t2, $a1
  div $t1, $t2
  mflo $t3
  move $t4, $a1
  mul $t5, $t3, $t4
  sub $t6, $t0, $t5
  move $v0, $t6
  jr $ra

gcd:
  move $t0, $a0
  li $t1, 0
  beq $t0, $t1, label0
  j label1
  li $t3, 0
  bne $t2, $t3, label0
  j label1
label0:
  move $t4, $a1
  move $v0, $t4
  jr $ra
  j label2
label1:
label2:
  move $t5, $a1
  move $t6, $a0
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
  move $a0, $t5
  move $a1, $t6
  jal mod
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
  move $t7, $v0
  move $s0, $a0
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
  move $a0, $t7
  move $a1, $s0
  jal gcd
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
  move $s1, $v0
  move $v0, $s1
  jr $ra

lcm:
  move $t0, $a0
  move $t1, $a1
  mul $t2, $t0, $t1
  move $t3, $a0
  move $t4, $a1
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
  move $a0, $t3
  move $a1, $t4
  jal gcd
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
  move $t5, $v0
  div $t2, $t5
  mflo $t6
  move $v0, $t6
  jr $ra

main:
  li $t0, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  move $t2, $t1
  li $t3, 0
  move $t4, $t3
label3:
  move $t5, $t4
  move $t6, $t2
  blt $t5, $t6, label4
  j label5
  li $s0, 0
  bne $t7, $s0, label4
  j label5
label4:
  la $s2, a
  move $s1, $s2
  move $s3, $t4
  li $s5, 4
  mul $s4, $s3, $s5
  add $s6, $s1, $s4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s7, $v0
  sw $s7, 0($s6)
  la $t9, b
  move $t8, $t9
  move $s0, $t4
  sw $t1, _spill_0
  li $t1, 4
  mul $s5, $s0, $t1
  sw $t3, _spill_1
  add $t3, $t8, $s5
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  sw $t1, 0($t3)
  sw $t5, _spill_2
  move $t5, $t4
  sw $t6, _spill_3
  li $t6, 1
  sw $t7, _spill_4
  add $t7, $t5, $t6
  move $t4, $t7
  j label3
label5:
  sw $s1, _spill_5
  move $s1, $t9
  sw $s3, _spill_6
  li $s3, 0
  sw $s4, _spill_7
  sw $s6, _spill_8
  li $s6, 4
  mul $s4, $s3, $s6
  sw $s7, _spill_9
  add $s7, $s1, $s4
  lw $s6, 0($s7)
  sw $t8, _spill_10
  move $t8, $s6
  sw $s0, _spill_11
  li $s0, 1
  move $t4, $s0
label6:
  sw $s5, _spill_12
  move $s5, $t4
  sw $t3, _spill_13
  move $t3, $t2
  blt $s5, $t3, label7
  j label8
  sw $t1, _spill_14
  sw $t5, _spill_15
  li $t5, 0
  bne $t1, $t5, label7
  j label8
label7:
  sw $t6, _spill_16
  move $t6, $t8
  move $t5, $t9
  sw $t7, _spill_17
  move $t7, $t4
  sw $s1, _spill_18
  sw $s3, _spill_19
  li $s3, 4
  mul $s1, $t7, $s3
  sw $s4, _spill_20
  add $s4, $t5, $s1
  lw $s3, 0($s4)
  sw $s7, _spill_21
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
  move $a0, $t6
  move $a1, $s3
  jal lcm
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
  move $s7, $v0
  move $t8, $s7
  sw $s6, _spill_22
  move $s6, $t4
  sw $s0, _spill_23
  li $s0, 1
  sw $s5, _spill_24
  add $s5, $s6, $s0
  move $t4, $s5
  j label6
label8:
  sw $t3, _spill_25
  li $t3, 0
  move $t4, $t3
label9:
  sw $t1, _spill_26
  move $t1, $t4
  sw $t6, _spill_27
  move $t6, $t2
  blt $t1, $t6, label10
  j label11
  sw $t5, _spill_28
  sw $t7, _spill_29
  li $t7, 0
  bne $t5, $t7, label10
  j label11
label10:
  sw $s1, _spill_30
  move $s1, $t0
  move $t7, $s2
  sw $s4, _spill_31
  move $s4, $t4
  sw $s3, _spill_32
  sw $s7, _spill_33
  li $s7, 4
  mul $s3, $s4, $s7
  sw $s6, _spill_34
  add $s6, $t7, $s3
  lw $s7, 0($s6)
  sw $s0, _spill_35
  move $s0, $t8
  sw $s5, _spill_36
  move $s5, $t9
  sw $t3, _spill_37
  move $t3, $t4
  sw $t1, _spill_38
  sw $t6, _spill_39
  li $t6, 4
  mul $t1, $t3, $t6
  sw $t5, _spill_40
  add $t5, $s5, $t1
  lw $t6, 0($t5)
  sw $s1, _spill_41
  div $s0, $t6
  mflo $s1
  sw $t7, _spill_42
  mul $t7, $s7, $s1
  sw $s4, _spill_43
  sw $s3, _spill_44
  lw $s3, _spill_41
  add $s4, $s3, $t7
  move $t0, $s4
  sw $s6, _spill_45
  move $s6, $t4
  sw $s7, _spill_46
  li $s7, 1
  sw $s0, _spill_47
  add $s0, $s6, $s7
  move $t4, $s0
  j label9
label11:
  sw $s5, _spill_48
  move $s5, $t0
  sw $t3, _spill_49
  move $t3, $t8
  sw $t1, _spill_50
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
  move $a0, $s5
  move $a1, $t3
  jal gcd
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
  move $t1, $v0
  sw $t5, _spill_51
  move $t5, $t1
  sw $t6, _spill_52
  move $t6, $t0
  sw $s1, _spill_53
  move $s1, $t5
  sw $t7, _spill_54
  div $t6, $s1
  mflo $t7
  move $t0, $t7
  sw $s4, _spill_55
  move $s4, $t8
  sw $s3, _spill_56
  move $s3, $t5
  sw $s6, _spill_57
  div $s4, $s3
  mflo $s6
  move $t8, $s6
  sw $s7, _spill_58
  move $s7, $t8
  sw $s0, _spill_59
  li $s0, 1
  beq $s7, $s0, label12
  j label13
  sw $s5, _spill_60
  sw $t3, _spill_61
  li $t3, 0
  bne $s5, $t3, label12
  j label13
label12:
  sw $t1, _spill_62
  move $t1, $t0
  move $a0, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label13:
  move $t3, $t0
  move $a0, $t3
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $t6, _spill_63
  move $t6, $t8
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label14:
  sw $s1, _spill_64
  li $s1, 0
  move $v0, $s1
  jr $ra
