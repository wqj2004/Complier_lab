FUNCTION main :
n := #5
DEC arr 20
DEC tmp 20
t0 := #0
i := t0
LABEL label0 :
t1 := i
t2 := n
IF t1 < t2 GOTO label1
GOTO label2
IF t3 != #0 GOTO label1
GOTO label2
LABEL label1 :
t4 := arr
t5 := i
t6 := t5 * #4
t7 := t4 + t6
READ t8
*t7 := t8
t10 := i
t11 := #1
t9 := t10 + t11
i := t9
GOTO label0
LABEL label2 :
t12 := #1
intv := t12
LABEL label3 :
t13 := intv
t14 := n
IF t13 < t14 GOTO label4
GOTO label5
IF t15 != #0 GOTO label4
GOTO label5
LABEL label4 :
t16 := #0
i := t16
LABEL label6 :
t17 := i
t19 := n
t21 := #2
t22 := intv
t20 := t21 * t22
t18 := t19 - t20
IF t17 <= t18 GOTO label7
GOTO label8
IF t23 != #0 GOTO label7
GOTO label8
LABEL label7 :
t24 := i
s1 := t24
t26 := s1
t27 := intv
t25 := t26 + t27
e1 := t25
t28 := s1
cur1 := t28
t29 := e1
s2 := t29
t31 := s2
t32 := intv
t30 := t31 + t32
e2 := t30
t33 := s2
cur2 := t33
LABEL label9 :
t34 := cur1
t35 := e1
IF t34 < t35 GOTO label12
GOTO label11
IF t36 != #0 GOTO label12
GOTO label11
LABEL label12 :
t37 := cur2
t38 := e2
IF t37 < t38 GOTO label10
GOTO label11
IF t39 != #0 GOTO label10
GOTO label11
IF t40 != #0 GOTO label10
GOTO label11
LABEL label10 :
t43 := arr
t44 := cur1
t45 := t44 * #4
t46 := t43 + t45
t41 := *t46
t47 := arr
t48 := cur2
t49 := t48 * #4
t50 := t47 + t49
t42 := *t50
IF t41 < t42 GOTO label13
GOTO label14
IF t51 != #0 GOTO label13
GOTO label14
LABEL label13 :
t52 := tmp
t53 := i
t54 := t53 * #4
t55 := t52 + t54
t57 := arr
t58 := cur1
t59 := t58 * #4
t60 := t57 + t59
t56 := *t60
*t55 := t56
t62 := cur1
t63 := #1
t61 := t62 + t63
cur1 := t61
GOTO label15
LABEL label14 :
t64 := tmp
t65 := i
t66 := t65 * #4
t67 := t64 + t66
t69 := arr
t70 := cur2
t71 := t70 * #4
t72 := t69 + t71
t68 := *t72
*t67 := t68
t74 := cur2
t75 := #1
t73 := t74 + t75
cur2 := t73
LABEL label15 :
t77 := i
t78 := #1
t76 := t77 + t78
i := t76
GOTO label9
LABEL label11 :
LABEL label16 :
t79 := cur1
t80 := e1
IF t79 < t80 GOTO label17
GOTO label18
IF t81 != #0 GOTO label17
GOTO label18
LABEL label17 :
t82 := tmp
t83 := i
t84 := t83 * #4
t85 := t82 + t84
t87 := arr
t88 := cur1
t89 := t88 * #4
t90 := t87 + t89
t86 := *t90
*t85 := t86
t92 := cur1
t93 := #1
t91 := t92 + t93
cur1 := t91
t95 := i
t96 := #1
t94 := t95 + t96
i := t94
GOTO label16
LABEL label18 :
LABEL label19 :
t97 := cur2
t98 := e2
IF t97 < t98 GOTO label20
GOTO label21
IF t99 != #0 GOTO label20
GOTO label21
LABEL label20 :
t100 := tmp
t101 := i
t102 := t101 * #4
t103 := t100 + t102
t105 := arr
t106 := cur2
t107 := t106 * #4
t108 := t105 + t107
t104 := *t108
*t103 := t104
t110 := cur2
t111 := #1
t109 := t110 + t111
cur2 := t109
t113 := i
t114 := #1
t112 := t113 + t114
i := t112
GOTO label19
LABEL label21 :
GOTO label6
LABEL label8 :
t117 := i
t118 := intv
t115 := t117 + t118
t116 := n
IF t115 < t116 GOTO label22
GOTO label23
IF t119 != #0 GOTO label22
GOTO label23
LABEL label22 :
t120 := i
s1 := t120
t122 := s1
t123 := intv
t121 := t122 + t123
e1 := t121
t124 := s1
cur1 := t124
t125 := e1
s2 := t125
t126 := n
e2 := t126
t127 := s2
cur2 := t127
LABEL label25 :
t128 := cur1
t129 := e1
IF t128 < t129 GOTO label28
GOTO label27
IF t130 != #0 GOTO label28
GOTO label27
LABEL label28 :
t131 := cur2
t132 := e2
IF t131 < t132 GOTO label26
GOTO label27
IF t133 != #0 GOTO label26
GOTO label27
IF t134 != #0 GOTO label26
GOTO label27
LABEL label26 :
t137 := arr
t138 := cur1
t139 := t138 * #4
t140 := t137 + t139
t135 := *t140
t141 := arr
t142 := cur2
t143 := t142 * #4
t144 := t141 + t143
t136 := *t144
IF t135 < t136 GOTO label29
GOTO label30
IF t145 != #0 GOTO label29
GOTO label30
LABEL label29 :
t146 := tmp
t147 := i
t148 := t147 * #4
t149 := t146 + t148
t151 := arr
t152 := cur1
t153 := t152 * #4
t154 := t151 + t153
t150 := *t154
*t149 := t150
t156 := cur1
t157 := #1
t155 := t156 + t157
cur1 := t155
t159 := i
t160 := #1
t158 := t159 + t160
i := t158
GOTO label31
LABEL label30 :
t161 := tmp
t162 := i
t163 := t162 * #4
t164 := t161 + t163
t166 := arr
t167 := cur2
t168 := t167 * #4
t169 := t166 + t168
t165 := *t169
*t164 := t165
t171 := cur2
t172 := #1
t170 := t171 + t172
cur2 := t170
t174 := i
t175 := #1
t173 := t174 + t175
i := t173
LABEL label31 :
GOTO label25
LABEL label27 :
LABEL label32 :
t176 := cur1
t177 := e1
IF t176 < t177 GOTO label33
GOTO label34
IF t178 != #0 GOTO label33
GOTO label34
LABEL label33 :
t179 := tmp
t180 := i
t181 := t180 * #4
t182 := t179 + t181
t184 := arr
t185 := cur1
t186 := t185 * #4
t187 := t184 + t186
t183 := *t187
*t182 := t183
t189 := cur1
t190 := #1
t188 := t189 + t190
cur1 := t188
t192 := i
t193 := #1
t191 := t192 + t193
i := t191
GOTO label32
LABEL label34 :
LABEL label35 :
t194 := cur2
t195 := e2
IF t194 < t195 GOTO label36
GOTO label37
IF t196 != #0 GOTO label36
GOTO label37
LABEL label36 :
t197 := tmp
t198 := i
t199 := t198 * #4
t200 := t197 + t199
t202 := arr
t203 := cur2
t204 := t203 * #4
t205 := t202 + t204
t201 := *t205
*t200 := t201
t207 := cur2
t208 := #1
t206 := t207 + t208
cur2 := t206
t210 := i
t211 := #1
t209 := t210 + t211
i := t209
GOTO label35
LABEL label37 :
GOTO label24
LABEL label23 :
LABEL label38 :
t212 := i
t213 := n
IF t212 < t213 GOTO label39
GOTO label40
IF t214 != #0 GOTO label39
GOTO label40
LABEL label39 :
t215 := tmp
t216 := i
t217 := t216 * #4
t218 := t215 + t217
t220 := arr
t221 := i
t222 := t221 * #4
t223 := t220 + t222
t219 := *t223
*t218 := t219
t225 := i
t226 := #1
t224 := t225 + t226
i := t224
GOTO label38
LABEL label40 :
LABEL label24 :
t227 := #0
i := t227
LABEL label41 :
t228 := i
t229 := n
IF t228 < t229 GOTO label42
GOTO label43
IF t230 != #0 GOTO label42
GOTO label43
LABEL label42 :
t231 := arr
t232 := i
t233 := t232 * #4
t234 := t231 + t233
t236 := tmp
t237 := i
t238 := t237 * #4
t239 := t236 + t238
t235 := *t239
*t234 := t235
t241 := i
t242 := #1
t240 := t241 + t242
i := t240
GOTO label41
LABEL label43 :
t244 := intv
t245 := #2
t243 := t244 * t245
intv := t243
GOTO label3
LABEL label5 :
t246 := #0
i := t246
LABEL label44 :
t247 := i
t248 := n
IF t247 < t248 GOTO label45
GOTO label46
IF t249 != #0 GOTO label45
GOTO label46
LABEL label45 :
t251 := arr
t252 := i
t253 := t252 * #4
t254 := t251 + t253
t250 := *t254
WRITE t250
t256 := i
t257 := #1
t255 := t256 + t257
i := t255
GOTO label44
LABEL label46 :
t258 := #0
RETURN t258
