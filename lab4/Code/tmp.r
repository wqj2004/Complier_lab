FUNCTION squaRever :
PARAM num
flag := #0
DEC array 12
j := #0
t0 := array
t1 := #0
t2 := t1 * #4
t3 := t0 + t2
t5 := num
t6 := #100
t4 := t5 / t6
*t3 := t4
t7 := array
t8 := #1
t9 := t8 * #4
t10 := t7 + t9
t14 := num
t15 := #10
t12 := t14 / t15
t16 := #10
t18 := array
t19 := #0
t20 := t19 * #4
t21 := t18 + t20
t17 := *t21
t13 := t16 * t17
t11 := t12 - t13
*t10 := t11
t22 := array
t23 := #2
t24 := t23 * #4
t25 := t22 + t24
t29 := num
t31 := #100
t33 := array
t34 := #0
t35 := t34 * #4
t36 := t33 + t35
t32 := *t36
t30 := t31 * t32
t27 := t29 - t30
t37 := #10
t39 := array
t40 := #1
t41 := t40 * #4
t42 := t39 + t41
t38 := *t42
t28 := t37 * t38
t26 := t27 - t28
*t25 := t26
t45 := array
t46 := #0
t47 := t46 * #4
t48 := t45 + t47
t43 := *t48
t49 := array
t50 := #2
t51 := t50 * #4
t52 := t49 + t51
t44 := *t52
IF t43 != t44 GOTO label0
GOTO label1
LABEL label0 :
t53 := #0
flag := t53
GOTO label2
LABEL label1 :
LABEL label3 :
t54 := j
t55 := #12
IF t54 < t55 GOTO label4
GOTO label5
LABEL label4 :
t60 := j
t61 := j
t58 := t60 * t61
t59 := num
t56 := t58 - t59
t57 := #0
IF t56 == t57 GOTO label6
GOTO label7
LABEL label6 :
t62 := #1
flag := t62
GOTO label8
LABEL label7 :
LABEL label8 :
t64 := j
t65 := #1
t63 := t64 + t65
j := t63
GOTO label3
LABEL label5 :
LABEL label2 :
t66 := flag
t67 := #1
IF t66 == t67 GOTO label9
GOTO label10
LABEL label9 :
t68 := #1
RETURN t68
GOTO label11
LABEL label10 :
t69 := #0
RETURN t69
LABEL label11 :
FUNCTION main :
i := #100
LABEL label12 :
t70 := i
t71 := #150
IF t70 < t71 GOTO label13
GOTO label14
LABEL label13 :
t74 := i
ARG t74
t72 := CALL squaRever
t73 := #1
IF t72 == t73 GOTO label15
GOTO label16
LABEL label15 :
t75 := i
WRITE t75
GOTO label17
LABEL label16 :
LABEL label17 :
t77 := i
t78 := #1
t76 := t77 + t78
i := t76
GOTO label12
LABEL label14 :
t79 := #0
RETURN t79
