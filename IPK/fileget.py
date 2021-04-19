#Emma Krompascikova
#xkromp00
#IPK project no. 1

import socket
import time
import sys, getopt

arguments = sys.argv[1:]
shortOptions = "n:f:"

#checking for valid arguments
try:
    optlist, args = getopt.getopt(arguments, shortOptions)
except:
    sys.stderr.write("neplatny vstup")
    exit(-1)

#checking for enough number of valid arguments
if len(sys.argv) <= 3:
    sys.stderr.write("neplatny vstup, malo argumentov")
    exit(-1)

#function for downloading data
def func_downloading(name_of):

    #checking if path includes directories
    if "/" in name_of:
        name_of_file = name_of.rsplit("/", 1)[1]
    else:
        name_of_file = name_of

    sock = socket.socket(socket.AF_INET, # Internet
                        socket.SOCK_DGRAM) # UDP
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

    sock.settimeout(60)
    try:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    except:
        sys.stderr.write("nothing received/error")
        exit(-1)
    
    status, address = data.decode().split(" ")

    #if status is no OK, throw error
    if (status != "OK"):
        sys.stderr.write("status isnt OK")
        exit(-1)

    TCP_IP = address.split(":")[0]
    TCP_PORT = int(address.split(":")[1])
    BUFFER_SIZE = 1024
    TCP_MESSAGE = b"GET " + name_of.encode() + b" FSP/1.0\r\nHostname: " + server_name.encode() + b"\r\nAgent: xkromp00\r\n\r\n"

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    s.send(TCP_MESSAGE)

    data = b""

    s.settimeout(60)

    try:
        while True:
            fragment = s.recv(BUFFER_SIZE)
            if len(fragment) == 0:
                break
            else:
                data = data + fragment
    except:
        sys.stderr.write("failed while downloading")
        exit(-1)        
    
    s.close()

    into_file = data.split("\r\n\r\n".encode(), 1)[1]

    f = open(name_of_file, "wb")
    f.write(into_file)
    f.close()

    #looking in header for success, if not found then error
    if "Success" not in data.split("\r\n\r\n".encode(), 1)[0].decode():
        sys.stderr.write("The specified file was not found")
        exit(-1)

#both -n -f and -f -n are available
if optlist[0][0] == "-n" and optlist[1][0] == "-f":
    parts = optlist[0][1].split(":", 1)
    UDP_IP = parts[0]
    UDP_PORT = int(parts[1])
    server_name_with_path = optlist[1][1].split("://", 1)[1]
    server_name = server_name_with_path.split("/", 1)[0]
    path = server_name_with_path.split("/", 1)[1]
    
    
elif optlist[1][0] == "-n" and optlist[0][0] == "-f":
    parts = optlist[1][1].split(":", 1)
    UDP_IP = parts[0]
    UDP_PORT = int(parts[1])
    server_name_with_path = optlist[0][1].split("://", 1)[1]
    server_name = server_name_with_path.split("/", 1)[0]
    path = server_name_with_path.split("/", 1)[1]

MESSAGE = b"WHEREIS " + server_name.encode()

#calling function to download
if path == "*":
    func_downloading("index")

    file_read = open("index", "r")
    content = file_read.read()
    content_list = content.splitlines()
    file_read.close()
    
    for each in content_list:
        
        func_downloading(each) 
else: 

    func_downloading(path)