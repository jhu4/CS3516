import sys
import socket
import threading
import time
import string
import select

class Server:
	def __init__(self,port):
		self.host=''
		self.port=port
	def opensocket(self):
		try:
			self.sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			print "Binding socket on port "+str(port)
			self.sock.bind((self.host,self.port))
		except socket.error:
			print 'Failed to creat socket'
			if self.sock:
				self.sock.close()
			sys.exit()

	def run(self):
		self.opensocket()
		self.sock.listen(50)
		input = [self.sock]
		while True:
			inputready,outputready,exceptready=select.select(input,[],[]) 
			for s in inputready:
				if s==self.sock:
					c=ServerThread(self.sock.accept())
					c.daemon=True
					c.start()

		self.sock.close()

def headerWriter(filesize,type):
	sizestr=str(filesize)
	header='HTTP/1.1 200 OK\r\n'
	header+='Content-Type: '+type+' charset=UTF-8\r\n'
	header+='Content-Length: '+sizestr+'\r\n\r\n'
	return header

class ServerThread(threading.Thread):
	def __init__(self,(client,address)):
		threading.Thread.__init__(self)
		print "Client address is"+str(address)
		self.client=client
		self.address=address
		self.size=1024
	def run(self):
		while True:
			try:
				data = self.client.recv(self.size)
				
				if data:
					print data
					msg=string.split(data,'\r\n')
					# print 'first line: '+str(msg[0])
					request=string.split(msg[0],' ')
					# print 'request: '+str(request)

					# if it is a valid HTTP request
					if request[0]=='GET' and request[2]=='HTTP/1.1':
							contents = ""
							with open('TMGD.html','r') as file:
								for line in file:
									contents+=line
							content=headerWriter(len(contents),'text/html')
							self.client.send(content)
							with open('TMGD.html','r') as file:
								for line in file:
									self.client.send(line)
							self.client.close()
							break

					else:
						self.client.send('HTTP/1.1 404 Not Found\r\n\r\n')
						self.client.close()
						break

				else:
					raise error('CLinet disconnected')
			except:
				self.client.close()
				break


if __name__=="__main__":
	if len(sys.argv)==2:
		try:
			port=int(sys.argv[1])
		except:
			sys.exit()
	else:
		print "Invalid inputs"
		sys.exit()

	s = Server(port)
	s.run()

