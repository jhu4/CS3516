import sys

def split(data):
	num=0
	for c in data:
		if c=='/':
			start=num
			break
		num+=1
	for c in data[start:]:
		if c==' ':
			stop=num
		num+=1
	return data[start:stop]

print split(sys.argv[1])