import time
import os

import sys

for line in sys.stdin:
    if 'Exit' == line.rstrip():
        break
    print(f'Processing Message from sys.stdin *****{line}*****')
print("Done")

time.sleep(1)
print("Hello World!")
print(os.environ)
