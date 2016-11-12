import sys
import socket

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

host,sub=parsestring(sys.argv[1])
print host
print sub
if checkvalidity(host):
	print 'Validity True'
else:
	print 'Validity False'
