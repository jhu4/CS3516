import sys
import string


contents = ""
with open('Google.html','r') as file:
	for line in file:
		contents+=line
sizestr=str(len(contents))
print sizestr