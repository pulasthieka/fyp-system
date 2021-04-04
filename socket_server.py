import socket
 
s = socket.socket()         
 
s.bind(('0.0.0.0', 8090 ))
s.listen(0)                 
 
while True:
 
    client, addr = s.accept()
    if(client):
        print(client, addr)
    while True:
        content = client.recv(256)
 
        if len(content) ==0:
           print('empty')
           break
 
        else:
            print(content)
 
    print("Closing connection")
    client.close()