# Собрать

```shell
cmake --build cmake-build-debug --target hw
```

# Запустить

```shell
./cmake-build-debug/hw Sort.bc
```

# Пример вывода

```
Bytecode:
String table size       : 10
Global area size        : 0
Number of public symbols: 1
Public symbols          :
   0x00000000: main
Code:
0x00000000:	BEGIN	2 0
0x00000009:	LINE	25
0x0000000e:	LINE	27
0x00000013:	CONST	10000
0x00000018:	CALL	0x0000002b 1
0x00000021:	CALL	0x00000075 1
0x0000002a:	END
0x0000002b:	BEGIN	1 0
0x00000034:	LINE	24
0x00000039:	LD	A(0)
0x0000003e:	CJMPz	0x0000006a
0x00000043:	LD	A(0)
0x00000048:	LD	A(0)
0x0000004d:	CONST	1
0x00000052:	BINOP	-
0x00000053:	CALL	0x0000002b 1
0x0000005c:	SEXP	cons 2
0x00000065:	JMP	0x00000074
0x0000006a:	CONST	0
0x0000006f:	JMP	0x00000074
0x00000074:	END
0x00000075:	BEGIN	1 0
0x0000007e:	LINE	18
0x00000083:	LINE	20
0x00000088:	LD	A(0)
0x0000008d:	CALL	0x00000097 1
0x00000096:	END
0x00000097:	BEGIN	1 1
0x000000a0:	LINE	14
0x000000a5:	LD	A(0)
0x000000aa:	CALL	0x0000015f 1
0x000000b3:	DUP
0x000000b4:	DUP
0x000000b5:	ARRAY	2
0x000000ba:	CJMPnz	0x000000c5
0x000000bf:	DROP
0x000000c0:	JMP	0x00000106
0x000000c5:	DUP
0x000000c6:	CONST	0
0x000000cb:	ELEM
0x000000cc:	CONST	1
0x000000d1:	BINOP	==
0x000000d2:	CJMPz	0x000000bf
0x000000d7:	DUP
0x000000d8:	CONST	1
0x000000dd:	ELEM
0x000000de:	DROP
0x000000df:	DROP
0x000000e0:	DUP
0x000000e1:	CONST	1
0x000000e6:	ELEM
0x000000e7:	ST	L(0)
0x000000ec:	DROP
0x000000ed:	DROP
0x000000ee:	LINE	15
0x000000f3:	LD	L(0)
0x000000f8:	CALL	0x00000097 1
0x00000101:	JMP	0x0000015e
0x00000106:	DUP
0x00000107:	DUP
0x00000108:	ARRAY	2
0x0000010d:	CJMPnz	0x00000118
0x00000112:	DROP
0x00000113:	JMP	0x00000150
0x00000118:	DUP
0x00000119:	CONST	0
0x0000011e:	ELEM
0x0000011f:	CONST	0
0x00000124:	BINOP	==
0x00000125:	CJMPz	0x00000112
0x0000012a:	DUP
0x0000012b:	CONST	1
0x00000130:	ELEM
0x00000131:	DROP
0x00000132:	DROP
0x00000133:	DUP
0x00000134:	CONST	1
0x00000139:	ELEM
0x0000013a:	ST	L(0)
0x0000013f:	DROP
0x00000140:	DROP
0x00000141:	LINE	16
0x00000146:	LD	L(0)
0x0000014b:	JMP	0x0000015e
0x00000150:	FAIL	14 9
0x00000159:	JMP	0x0000015e
0x0000015e:	END
0x0000015f:	BEGIN	1 6
0x00000168:	LINE	3
0x0000016d:	LD	A(0)
0x00000172:	DUP
0x00000173:	DUP
0x00000174:	TAG	cons 2
0x0000017d:	CJMPnz	0x00000188
0x00000182:	DROP
0x00000183:	JMP	0x000002de
0x00000188:	DUP
0x00000189:	CONST	0
0x0000018e:	ELEM
0x0000018f:	DROP
0x00000190:	DUP
0x00000191:	CONST	1
0x00000196:	ELEM
0x00000197:	DUP
0x00000198:	TAG	cons 2
0x000001a1:	CJMPnz	0x000001ac
0x000001a6:	DROP
0x000001a7:	JMP	0x00000182
0x000001ac:	DUP
0x000001ad:	CONST	0
0x000001b2:	ELEM
0x000001b3:	DROP
0x000001b4:	DUP
0x000001b5:	CONST	1
0x000001ba:	ELEM
0x000001bb:	DROP
0x000001bc:	DROP
0x000001bd:	DROP
0x000001be:	DUP
0x000001bf:	CONST	0
0x000001c4:	ELEM
0x000001c5:	ST	L(3)
0x000001ca:	DROP
0x000001cb:	DUP
0x000001cc:	CONST	1
0x000001d1:	ELEM
0x000001d2:	ST	L(2)
0x000001d7:	DROP
0x000001d8:	DUP
0x000001d9:	CONST	1
0x000001de:	ELEM
0x000001df:	CONST	0
0x000001e4:	ELEM
0x000001e5:	ST	L(1)
0x000001ea:	DROP
0x000001eb:	DUP
0x000001ec:	CONST	1
0x000001f1:	ELEM
0x000001f2:	CONST	1
0x000001f7:	ELEM
0x000001f8:	ST	L(0)
0x000001fd:	DROP
0x000001fe:	DROP
0x000001ff:	LINE	5
0x00000204:	LD	L(3)
0x00000209:	LD	L(1)
0x0000020e:	BINOP	>
0x0000020f:	CJMPz	0x00000258
0x00000214:	CONST	1
0x00000219:	LINE	6
0x0000021e:	LD	L(1)
0x00000223:	LD	L(3)
0x00000228:	LD	L(0)
0x0000022d:	SEXP	cons 2
0x00000236:	CALL	0x0000015f 1
0x0000023f:	CONST	1
0x00000244:	ELEM
0x00000245:	SEXP	cons 2
0x0000024e:	CALL	Barray	2
0x00000253:	JMP	0x000002fa
0x00000258:	LINE	7
0x0000025d:	LD	L(2)
0x00000262:	CALL	0x0000015f 1
0x0000026b:	DUP
0x0000026c:	DUP
0x0000026d:	ARRAY	2
0x00000272:	CJMPnz	0x0000027d
0x00000277:	DROP
0x00000278:	JMP	0x000002cb
0x0000027d:	DUP
0x0000027e:	CONST	0
0x00000283:	ELEM
0x00000284:	DROP
0x00000285:	DUP
0x00000286:	CONST	1
0x0000028b:	ELEM
0x0000028c:	DROP
0x0000028d:	DROP
0x0000028e:	DUP
0x0000028f:	CONST	0
0x00000294:	ELEM
0x00000295:	ST	L(5)
0x0000029a:	DROP
0x0000029b:	DUP
0x0000029c:	CONST	1
0x000002a1:	ELEM
0x000002a2:	ST	L(4)
0x000002a7:	DROP
0x000002a8:	DROP
0x000002a9:	LD	L(5)
0x000002ae:	LD	L(3)
0x000002b3:	LD	L(4)
0x000002b8:	SEXP	cons 2
0x000002c1:	CALL	Barray	2
0x000002c6:	JMP	0x000002fa
0x000002cb:	FAIL	7 17
0x000002d4:	JMP	0x000002fa
0x000002de:	DUP
0x000002df:	DROP
0x000002e0:	DROP
0x000002e1:	CONST	0
0x000002e6:	LINE	9
0x000002eb:	LD	A(0)
0x000002f0:	CALL	Barray	2
0x000002f5:	JMP	0x000002fa
0x000002fa:	END

Most common idioms:
Rank Occurs	Idiom
   1:    31	DROP
   2:    28	DUP
   3:    21	ELEM
   4:    16	CONST	1
   5:    13	CONST	1
           	ELEM
   6:    11	DROP
           	DUP
   7:    11	CONST	0
   8:    11	DUP
           	CONST	1
   9:    10	DROP
           	DROP
  10:     8	CONST	0
           	ELEM
  11:     7	LD	A(0)
  12:     7	DUP
           	CONST	0
  13:     7	ELEM
           	DROP
  14:     5	END
  15:     4	SEXP	cons 2
  16:     4	DUP
           	DUP
  17:     4	JMP	0x000002fa
  18:     3	CALL	Barray	2
  19:     3	CALL	Barray	2
           	JMP	0x000002fa
  20:     3	ARRAY	2
  21:     3	DUP
           	ARRAY	2
  22:     3	ST	L(0)
  23:     3	ELEM
           	ST	L(0)
  24:     3	LD	L(3)
  25:     3	ST	L(0)
           	DROP
  26:     3	LD	L(0)
  27:     3	CALL	0x0000015f 1
  28:     3	JMP	0x0000015e
  29:     2	DUP
           	TAG	cons 2
  30:     2	CALL	0x0000015f 1
           	DUP
  31:     2	BINOP	==
  32:     2	TAG	cons 2
  33:     2	CALL	0x00000097 1
  34:     2	SEXP	cons 2
           	CALL	Barray	2
  35:     2	JMP	0x00000074
  36:     2	BEGIN	1 0
  37:     2	CALL	0x0000002b 1
  38:     2	LD	L(1)
  39:     2	ELEM
           	CONST	0
  40:     2	ELEM
           	CONST	1
  41:     1	CONST	0
           	LINE	9
  42:     1	CJMPz	0x00000258
           	CONST	1
  43:     1	LD	A(0)
           	CALL	Barray	2
  44:     1	LD	A(0)
           	CJMPz	0x0000006a
  45:     1	LD	A(0)
           	LD	A(0)
  46:     1	LD	A(0)
           	CONST	1
  47:     1	CJMPz	0x0000006a
           	LD	A(0)
  48:     1	CONST	0
           	JMP	0x00000074
  49:     1	ST	L(2)
           	DROP
  50:     1	ST	L(3)
           	DROP
  51:     1	ST	L(4)
           	DROP
  52:     1	ST	L(5)
           	DROP
  53:     1	CJMPz	0x00000112
           	DUP
  54:     1	CJMPz	0x000000bf
           	DUP
  55:     1	CJMPnz	0x0000027d
           	DROP
  56:     1	CJMPnz	0x000001ac
           	DROP
  57:     1	BINOP	-
           	CALL	0x0000002b 1
  58:     1	LD	L(5)
           	LD	L(3)
  59:     1	LINE	3
           	LD	A(0)
  60:     1	CONST	1
           	LINE	6
  61:     1	ELEM
           	SEXP	cons 2
  62:     1	LD	L(0)
           	JMP	0x0000015e
  63:     1	LD	L(1)
           	LD	L(3)
  64:     1	LD	L(3)
           	LD	L(0)
  65:     1	LD	L(3)
           	LD	L(1)
  66:     1	LD	L(3)
           	LD	L(4)
  67:     1	BEGIN	2 0
           	LINE	25
  68:     1	LD	L(0)
           	CALL	0x00000097 1
  69:     1	LD	L(2)
           	CALL	0x0000015f 1
  70:     1	LD	L(4)
           	SEXP	cons 2
  71:     1	LD	A(0)
           	CALL	0x0000015f 1
  72:     1	LD	A(0)
           	CALL	0x00000097 1
  73:     1	BEGIN	1 0
           	LINE	18
  74:     1	BEGIN	1 0
           	LINE	24
  75:     1	BEGIN	1 1
           	LINE	14
  76:     1	BEGIN	1 6
           	LINE	3
  77:     1	LD	L(0)
           	SEXP	cons 2
  78:     1	CALL	0x0000015f 1
           	CONST	1
  79:     1	CALL	0x00000097 1
           	JMP	0x0000015e
  80:     1	TAG	cons 2
           	CJMPnz	0x00000188
  81:     1	TAG	cons 2
           	CJMPnz	0x000001ac
  82:     1	FAIL	7 17
           	JMP	0x000002fa
  83:     1	FAIL	14 9
           	JMP	0x0000015e
  84:     1	SEXP	cons 2
           	CALL	0x0000015f 1
  85:     1	CALL	0x0000002b 1
           	SEXP	cons 2
  86:     1	CALL	0x0000002b 1
           	CALL	0x00000075 1
  87:     1	LINE	14
           	LD	A(0)
  88:     1	CALL	0x00000097 1
           	END
  89:     1	ARRAY	2
           	CJMPnz	0x00000118
  90:     1	ARRAY	2
           	CJMPnz	0x0000027d
  91:     1	ARRAY	2
           	CJMPnz	0x000000c5
  92:     1	DROP
           	LD	L(5)
  93:     1	LINE	5
           	LD	L(3)
  94:     1	LINE	6
           	LD	L(1)
  95:     1	LINE	7
           	LD	L(2)
  96:     1	LINE	9
           	LD	A(0)
  97:     1	CALL	0x00000075 1
           	END
  98:     1	LINE	15
           	LD	L(0)
  99:     1	LINE	16
           	LD	L(0)
 100:     1	LINE	18
           	LINE	20
 101:     1	LINE	20
           	LD	A(0)
 102:     1	LINE	24
           	LD	A(0)
 103:     1	LINE	25
           	LINE	27
 104:     1	LINE	27
           	CONST	10000
 105:     1	CONST	10000
           	CALL	0x0000002b 1
 106:     1	SEXP	cons 2
           	JMP	0x00000074
 107:     1	LINE	5
 108:     1	CJMPz	0x0000006a
 109:     1	CJMPz	0x000000bf
 110:     1	CJMPnz	0x00000118
 111:     1	CJMPnz	0x0000027d
 112:     1	CJMPnz	0x00000188
 113:     1	CJMPnz	0x000001ac
 114:     1	CJMPnz	0x000000c5
 115:     1	LINE	3
 116:     1	CJMPz	0x00000258
 117:     1	LINE	6
 118:     1	LINE	7
 119:     1	LINE	9
 120:     1	LINE	14
 121:     1	LINE	15
 122:     1	LINE	16
 123:     1	LINE	18
 124:     1	LINE	20
 125:     1	LD	L(4)
 126:     1	BINOP	>
 127:     1	CONST	10000
 128:     1	JMP	0x00000106
 129:     1	JMP	0x00000150
 130:     1	JMP	0x00000182
 131:     1	JMP	0x000002cb
 132:     1	JMP	0x000002de
 133:     1	LD	L(2)
 134:     1	LINE	24
 135:     1	LD	L(5)
 136:     1	ST	L(1)
 137:     1	ST	L(2)
 138:     1	ST	L(3)
 139:     1	ST	L(4)
 140:     1	ST	L(5)
 141:     1	CJMPz	0x00000112
 142:     1	DROP
           	LINE	16
 143:     1	DROP
           	JMP	0x00000106
 144:     1	DROP
           	JMP	0x00000150
 145:     1	DROP
           	JMP	0x00000182
 146:     1	DROP
           	JMP	0x000002cb
 147:     1	DROP
           	JMP	0x000002de
 148:     1	BINOP	-
 149:     1	DROP
           	LINE	5
 150:     1	DROP
           	LINE	15
 151:     1	DROP
           	CONST	0
 152:     1	ELEM
           	ST	L(1)
 153:     1	ELEM
           	ST	L(2)
 154:     1	ELEM
           	ST	L(3)
 155:     1	ELEM
           	ST	L(4)
 156:     1	ELEM
           	ST	L(5)
 157:     1	LD	L(1)
           	BINOP	>
 158:     1	LD	A(0)
           	DUP
 159:     1	DUP
           	DROP
 160:     1	LINE	25
 161:     1	LINE	27
 162:     1	BEGIN	1 1
 163:     1	BEGIN	1 6
 164:     1	BEGIN	2 0
 165:     1	CALL	0x00000075 1
 166:     1	FAIL	7 17
 167:     1	FAIL	14 9
 168:     1	ST	L(1)
           	DROP
 169:     1	ELEM
           	DUP
 170:     1	BINOP	>
           	CJMPz	0x00000258
 171:     1	BINOP	==
           	CJMPz	0x00000112
 172:     1	BINOP	==
           	CJMPz	0x000000bf
 173:     1	CONST	0
           	BINOP	==
 174:     1	CONST	1
           	BINOP	-
 175:     1	CONST	1
           	BINOP	==
```
