FUNCTION mod :
PARAM x
PARAM y
t1 := x
t5 := x
t6 := y
t3 := t5 / t6
t4 := y
t2 := t3 * t4
t0 := t1 - t2
RETURN t0
FUNCTION gcd :
PARAM c
PARAM d
t7 := c
t8 := #0
IF t7 == t8 GOTO label0
GOTO label1
IF t9 != #0 GOTO label0
GOTO label1
LABEL label0 :
t10 := d
RETURN t10
GOTO label2
LABEL label1 :
LABEL label2 :
t13 := d
t14 := c
ARG t14
ARG t13
t12 := CALL mod
t15 := c
ARG t15
ARG t12
t11 := CALL gcd
RETURN t11
FUNCTION lcm :
PARAM e
PARAM f
t19 := e
t20 := f
t17 := t19 * t20
t21 := e
t22 := f
ARG t22
ARG t21
t18 := CALL gcd
t16 := t17 / t18
RETURN t16
FUNCTION main :
sum := #0
DEC a 200
DEC b 200
READ t23
n := t23
t24 := #0
i := t24
LABEL label3 :
t25 := i
t26 := n
IF t25 < t26 GOTO label4
GOTO label5
IF t27 != #0 GOTO label4
GOTO label5
LABEL label4 :
t28 := a
t29 := i
t30 := t29 * #4
t31 := t28 + t30
READ t32
*t31 := t32
t33 := b
t34 := i
t35 := t34 * #4
t36 := t33 + t35
READ t37
*t36 := t37
t39 := i
t40 := #1
t38 := t39 + t40
i := t38
GOTO label3
LABEL label5 :
t42 := b
t43 := #0
t44 := t43 * #4
t45 := t42 + t44
t41 := *t45
tmp := t41
t46 := #1
i := t46
LABEL label6 :
t47 := i
t48 := n
IF t47 < t48 GOTO label7
GOTO label8
IF t49 != #0 GOTO label7
GOTO label8
LABEL label7 :
t51 := tmp
t53 := b
t54 := i
t55 := t54 * #4
t56 := t53 + t55
t52 := *t56
ARG t52
ARG t51
t50 := CALL lcm
tmp := t50
t58 := i
t59 := #1
t57 := t58 + t59
i := t57
GOTO label6
LABEL label8 :
t60 := #0
i := t60
LABEL label9 :
t61 := i
t62 := n
IF t61 < t62 GOTO label10
GOTO label11
IF t63 != #0 GOTO label10
GOTO label11
LABEL label10 :
t65 := sum
t69 := a
t70 := i
t71 := t70 * #4
t72 := t69 + t71
t67 := *t72
t73 := tmp
t75 := b
t76 := i
t77 := t76 * #4
t78 := t75 + t77
t74 := *t78
t68 := t73 / t74
t66 := t67 * t68
t64 := t65 + t66
sum := t64
t80 := i
t81 := #1
t79 := t80 + t81
i := t79
GOTO label9
LABEL label11 :
t83 := sum
t84 := tmp
ARG t84
ARG t83
t82 := CALL gcd
g := t82
t86 := sum
t87 := g
t85 := t86 / t87
sum := t85
t89 := tmp
t90 := g
t88 := t89 / t90
tmp := t88
t91 := tmp
t92 := #1
IF t91 == t92 GOTO label12
GOTO label13
IF t93 != #0 GOTO label12
GOTO label13
LABEL label12 :
t94 := sum
WRITE t94
GOTO label14
LABEL label13 :
t95 := sum
WRITE t95
t96 := tmp
WRITE t96
LABEL label14 :
t97 := #0
RETURN t97
