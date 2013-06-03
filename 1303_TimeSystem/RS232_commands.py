import math


commands = [106, 130, 131, 135, 136, 140, 149, 150, 152, 154, 156, 157, 159]

commands_hex = {}
for com in commands:
	commands_hex[com] = hex(com)
	print str(com) + ' --> ' + commands_hex[com]


mantisa = int('0xFC17FD9', 0)
exp = int('0x3F', 0)


print '0x884 = ' + str(int('0x884', 0)) + ' ^ ' + str(int('0x840', 0))
print '0x488 = ' + str(int('0x488', 0)) + ' ^ ' + str(int('0x400', 0))
print '0x884840 = ' + str(int('0x884840', 0))
print '0x404888 = ' + str(int('0x404888', 0))