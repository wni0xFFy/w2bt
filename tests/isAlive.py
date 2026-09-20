import os
import subprocess
import time
import socket

server = subprocess.Popen(
    ["../bin/w2bt"],
    stdout=subprocess.PIPE, 
    stderr=subprocess.STDOUT, 
    text=True                  
)

time.sleep(2)

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2) 
    result = s.connect_ex(("127.0.0.1", 25656))
    s.close()  
finally:
    server.terminate()
    server_stdout, _ = server.communicate()
    if 'CONNECTION CLOSED BY CLIENT' in server_stdout:
        print("PASSED")
    else:
        print("NOT PASSED")
