.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
x: .space 20
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

main:
  li $t0, 0                   #t0 = i
  li $t1, 5                   #t1 = 5
label0:
  move $t2, $t0
  move $t3, $t1
  blt $t2, $t3, label1        #比较
  j label2
label1:
  la $t5, x
  move $t4, $t5               #t4=t5=x
  move $t6, $t0               #t6=i
  li $s0, 4                   #s0=4
  mul $t7, $t6, $s0           #t7=4*i
  add $s1, $t4, $t7           #s1 = x+4*i
  move $s2, $t0               #s2 = i
  li $s3, 2                   #s3 = 2
  add $s4, $s2, $s3           #s4 = i+2
  sw $s4, 0($s1)              #x[i] = i+2
  move $s5, $t5               #s5 = x
  move $s6, $t0               #s6 = i
  li $s0, 4                   #s0 = 4
  mul $s7, $s6, $s0           #s7 = i*4
  add $t8, $s5, $s7           #t8 = x+i*4
  lw $t9, 0($t8)              #t9 = x[i]
  move $t0, $t9               #t0 = t9 = x[i]
  j label0
label2:
  li $s0, 0
  move $v0, $s0
  jr $ra


