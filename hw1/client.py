import socket   #for sockets
import sys  #for exit
import string #for parse files
import time #for RTT

#parse the address into address and subdirectory
def parsestring(address):
	index=0
	#if the address start with http:// or https://
	if len(address)>7 and address[:7]=='http://':
		address=address[7:]
	if len(address)>8 and address[:8]=='https://':
		address=address[8:]
	#split the address into host and subdirectory
	for c in address:
		if c=='/':
			subdirectory=address[index+1:]
			if len(subdirectory)==0:
				subdirectory=None
			host=address[:index]
			return (host,subdirectory)
		index+=1

	return (address,None)

#check the validity of a host
def checkvalidity(host):
	#if host is possibly an ip address
	if host[-1]=='.':
		return False
	#this might be an ip address
	if host[-1].isdigit():
		if len(host)<11 or len(host)>15:
			#not a valid ip address
			return False
		else:
			if host[:3].isdigit() and host[3]=='.' and host[4:7].isdigit() and host[7]=='.':
				amount=0
				pos=8
				count=8
				for c in host[8:]:
					if c=='.':
						amount+=1
						pos=count
					count+=1
				if amount!=1 or len(host[8:pos])>3 or len(host[8:pos])<1 or len(host[pos+1:])>3 or len(host[pos+1:])<1:
					return False
			else:
				return False
	#this might be a host name:
	else:
		amount=0
		for c in host:
			if c==".":
				amount+=1
		if amount>4:
			return False	
	return True

rttenable=False

length = len(sys.argv)
# there are only address and socket num
if length==3:
	add=sys.argv[1]
	host,path=parsestring(add)
	#if it is not a valid address
	if not checkvalidity(host):
		print 'Input address is not valid'
		sys.exit()
	try:
		port=int(sys.argv[2])
	except:
		print "Port number is not integer number"
		sys.exit()

# there are specification, address and socket num
elif length==4:
	if sys.argv[1]=="-p":
		rttenable=True
		add=sys.argv[2]
		host,path=parsestring(add)
		#if it is not a valid address
		if not checkvalidity(host):
			print 'Input address is not valid'
			sys.exit()
		if sys.argv[3].isdigit():
			port=int(sys.argv[3])
		else:
			print "Not integer number"
			sys.exit()
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
if path:
	print path
	message='GET '+'/'+path+' HTTP/1.1\r\n\r\n'
else:
	message='GET / HTTP/1.1\r\n\r\n'

if rttenable:
	starttime=time.time()

try:
	#send request to the host
	s.sendall(message)
except socket.error:
	print 'Failed to send message to the server'
	s.close()
	sys.exit()



while True:
	data=s.recv(1024)
	if data:
		print data
	else:
		break 



#if optional is made by the user...print the RTT time
if rttenable:
	print 'RTT time: '+str(1000*(time.time()-starttime))+' milliseconds'
	print '=============================================\n'

print data

s.close()

