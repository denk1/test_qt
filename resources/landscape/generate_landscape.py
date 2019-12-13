import struct

value = 75.0
f = open('plane.raw', 'wb')
for i in range(257*257):
	f.write(struct.pack("f", value))
f.close()
