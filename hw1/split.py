import sys
import string
import os


hi='TMGD.html'

f=open(hi,'rb')
f.seek(0,2)
size=f.tell()
f.close()
print size
print os.stat(hi).st_size