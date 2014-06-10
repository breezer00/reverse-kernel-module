reverse-kernel-module
=====================

Beginner's Linux kernel hack.

A simple kernel module that creates a /dev/reverse device, a string written 
to this device is read back with the word order reversed.

See: http://www.linuxvoice.com/be-a-kernel-hacker/


`git clone https://github.com/breezer00/reverse-kernel-module.git`  
`make`  
`sudo insmod reverse.ko`   

