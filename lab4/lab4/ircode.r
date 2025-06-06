FUNCTION mod :
PARAM number2
PARAM m2
t0 := number2
t4 := number2
t5 := m2
t2 := t4 / t5
t3 := m2
t1 := t2 * t3
result := t0 - t1
result2 := result
t6 := result
RETURN t6
FUNCTION power :
PARAM base1
PARAM p1
t9 := #1
t10 := p1
t7 := t9 + t10
t8 := p1
ret1 := t7 - t8
LABEL label0 :
t11 := p1
t21 := ret1
t22 := ret1
t19 := t21 - t22
t20 := #90
t17 := t19 + t20
t18 := #89
t15 := t17 - t18
t16 := #1
t13 := t15 + t16
t14 := #2
t12 := t13 - t14
IF t11 > t12 GOTO label1
GOTO label2
IF t23 != #0 GOTO label1
GOTO label2
LABEL label1 :
t25 := ret1
t26 := base1
t24 := t25 * t26
ret1 := t24
t34 := #2
t35 := #1
t32 := t34 * t35
t33 := p1
t30 := t32 * t33
t36 := #1
t37 := p1
t31 := t36 * t37
t28 := t30 - t31
t29 := #1
t27 := t28 - t29
p1 := t27
GOTO label0
LABEL label2 :
t38 := ret1
RETURN t38
FUNCTION getNumDigits :
PARAM number3
ret3 := #0
t39 := number3
t40 := #0
IF t39 < t40 GOTO label3
GOTO label4
IF t41 != #0 GOTO label3
GOTO label4
LABEL label3 :
t43 := #1
t42 := #0 - t43
RETURN t42
GOTO label5
LABEL label4 :
LABEL label5 :
LABEL label6 :
t44 := number3
t45 := #0
IF t44 > t45 GOTO label7
GOTO label8
IF t46 != #0 GOTO label7
GOTO label8
LABEL label7 :
t48 := number3
t49 := #10
t47 := t48 / t49
number3 := t47
t51 := ret3
t52 := #2
t50 := t51 + t52
ret3 := t50
t54 := ret3
t55 := #2
t53 := t54 + t55
ret3 := t53
t57 := ret3
t58 := #3
t56 := t57 - t58
ret3 := t56
GOTO label6
LABEL label8 :
t59 := ret3
RETURN t59
FUNCTION isNarcissistic :
PARAM number4
t63 := #1
t64 := number4
t61 := t63 + t64
t62 := #1
t60 := t61 - t62
ARG t60
numDigits4 := CALL getNumDigits
sum4 := #0
n4 := number4
LABEL label9 :
t65 := n4
t66 := #0
IF t65 > t66 GOTO label10
GOTO label11
IF t67 != #0 GOTO label10
GOTO label11
LABEL label10 :
t69 := n4
ARG t69
t70 := #10
ARG t70
t68 := CALL mod
s4 := t68
t74 := n4
t75 := s4
t72 := t74 - t75
t73 := #10
t71 := t72 / t73
n4 := t71
t77 := sum4
t79 := s4
ARG t79
t80 := numDigits4
ARG t80
t78 := CALL power
t76 := t77 + t78
sum4 := t76
GOTO label9
LABEL label11 :
t81 := sum4
t82 := number4
IF t81 == t82 GOTO label12
GOTO label13
IF t83 != #0 GOTO label12
GOTO label13
LABEL label12 :
t84 := #1
RETURN t84
GOTO label14
LABEL label13 :
t85 := #0
RETURN t85
LABEL label14 :
FUNCTION main :
count := #0
i := #300
LABEL label15 :
t86 := i
t87 := #500
IF t86 < t87 GOTO label16
GOTO label17
IF t88 != #0 GOTO label16
GOTO label17
LABEL label16 :
t91 := i
ARG t91
t89 := CALL isNarcissistic
t90 := #1
IF t89 == t90 GOTO label18
GOTO label19
IF t92 != #0 GOTO label18
GOTO label19
LABEL label18 :
t93 := i
WRITE t93
t95 := count
t96 := #1
t94 := t95 + t96
count := t94
GOTO label20
LABEL label19 :
LABEL label20 :
t98 := i
t99 := #1
t97 := t98 + t99
i := t97
GOTO label15
LABEL label17 :
t100 := count
WRITE t100
t101 := count
RETURN t101
