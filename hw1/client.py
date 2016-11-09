import socket   #for sockets
import sys  #for exit
import string #for parse files
import time #for RTT

rttenable=False
ttlenable=False

length = len(sys.argv)
# there are only address and socket num
if length==3:
	host=sys.argv[1]
	try:
		port=int(sys.argv[2])
	except:
		print "Port number is not integer number"
		sys.exit()

# there are specification, address and socket num
elif length==4:
	if sys.argv[1]=="-p":
		rttenable=True
		host=sys.argv[2]
		if sys.argv[3].isdigit():
			port=int(sys.argv[3])
		else:
			print "Not integer number"
			sys.exit()
	elif sys.argv[1]=='--ttl':
		ttlenable=True
	else:
		print "unrecognized command"
		sys.exit()

else:
	print 'Invalid inputs'
	sys.exit()




try:
	#open a new socket
	s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
	print 'Failed to creat socket'
	if s:
		s.close()
	sys.exit()

try:
	#connect to the host
	s.connect((host,port))
except socket.error:
	print 'Failed to connect to the server'

print 'Connected to '+host+' by port '+str(port)
print '=============================================\n'
#send HTTP/1.1 GET message to the host
message = 'GET / HTTP/1.1\r\n\r\n'

if rttenable:
	starttime=time.time()

try:
	#send request to the host
	s.send(message)
except socket.error:
	print 'Failed to send message to the server'
	s.close()
	sys.exit()

#recieve pkg from the host
receive=s.recv(10000)
#if optional is made by the user...print the RTT time
if rttenable:
	print 'RTT time: '+str(1000*(time.time()-starttime))+' milliseconds'
	print '=============================================\n'

print receive
s.close()



def parsestring(address):
	index=0
	for c in address:
		if c=='/':
			subdirectory=address[index+1:]
			host=address[:index]
			return (host,subdirectory)
		index+=1

	return (address,None)
