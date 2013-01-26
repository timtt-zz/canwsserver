canwsserver - CAN WebSockets Server
===================================

(c) 2013 Tim Trampedach (tim@timtt.com)

Server to read CAN messages from Linux SocketCAN interface and relay them to a websockets interface. Compiles on Ubuntu Linux with default kernel (which has SocketCAN module already compiled). May require creation of virtual CAN device if not already created.