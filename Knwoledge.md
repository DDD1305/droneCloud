Stream socket (order) -> TCP
Datagram socket (not order) -> UDP 

Both use ip for routing

TCP it's when u want to be sure that the message arrive 

UDP it's when u want speed 

##

Two ways to store the two-byte hex number (ex b34f)

first way (everyone) : store first b3 follow by 4f, it's big endian also called Network Byte Order
second way (intel ) : store first 4f follow by b3, it's little endian 

How do the computer store ? It's Host byte order (little or big endian).

there are two types of numbers that u can convert : short (2 bytes) and long (4 bytes)

You use fonctions to translate :
- htons (h*ost* to n*etwork* s*hort*)
- htonl
- ntohs
- ntohl
