import sys
import socket
import threading
import time
import string
import errno

class Server:
	def __init__(self,port):
		self.host=''
		self.port=port
		
	def run(self):
		try:
			self.sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			print "Binding socket on port "+str(port)
			self.sock.bind((self.host,self.port))
		except socket.error:
			print 'Failed to creat socket'
			if self.sock:
				self.sock.close()
			sys.exit()
		self.sock.listen(50)
		while True:
			c=ServerThread(self.sock.accept())
			c.daemon=True
			c.run()
			# client,address=self.sock.accept()
			# try:
			# 	data = client.recv(1024)
				
			# 	if data:
			# 		print data
			# 		msg=string.split(data,'\r\n')
			# 		# print 'first line: '+str(msg[0])
			# 		request=string.split(msg[0],' ')
			# 		# print 'request: '+str(request)

			# 		# if it is a valid HTTP request
			# 		if request[0]=='GET' and request[2]=='HTTP/1.1':
			# 				contents = ""
			# 				with open('TMGD.html','r') as file:
			# 					for line in file:
			# 						contents+=line
			# 				sizestr=str(len(contents))
			# 				# print 'size',sizestr
			# 				# print 'request[1]',str(request[1])
			# 				content='HTTP/1.1 200 OK\r\n'
			# 				content+='Content-Type: text/html; charset=UTF-8\r\n'
			# 				content+='Content-Length: '+sizestr+'\r\n\r\n'
			# 				client.send(content)
			# 				print content
			# 				client.send(contents)
			# 				client.close()
			# 				# print('after ', i)
			# 				break

			# 		else:
			# 			client.send('HTTP/1.1 404 Not Found\r\n\r\n')
			# 			client.close()
			# 			break	

			# 	else:
			# 		raise error('CLinet disconnected')
			# except:
			# 	client.close()
			# 	break

class ServerThread(threading.Thread):
	def __init__(self,(client,address)):
		threading.Thread.__init__(self)
		self.client=client
		print "Client address is"+str(address)
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
							sizestr=str(len(contents))
							# print 'size',sizestr
							# print 'request[1]',str(request[1])
							content='HTTP/1.1 200 OK\r\n'
							content+='Content-Type: text/html; charset=UTF-8\r\n'
							content+='Content-Length: '+sizestr+'\r\n\r\n'
							self.client.send(content)
							print content
							self.client.send(contents)
							self.client.close()
							# print('after ', i)
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

