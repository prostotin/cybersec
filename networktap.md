### LAN tapping

So I was interested in the idea of packet sniffing one day. At first you might think that you could simply write a program that captures packets on the victim's computer.

But then you realize that this is very detectable. An unknown process running on the computer might make the victim suspicious, and delete the program, or reinstall their whole system. 

So how does one make sure that it never gets found out? 

## Use a network tap!

If you have physical access to the Ethernet cabling, you can implant a sneaky throwing-star-like device that can mirror every packet that goes through it and is virtually impossible to detect.

![tap](https://i.imgur.com/w1U7tjW.png)

Here's a simple diagram to explain our little "network".

![diagram](https://i.imgur.com/gn3bhT7.png)

And this is how it looks after we throw everything together:

![end](https://i.imgur.com/lqHQETY.png)

As you can see, with very little effort we are able to sniff the traffic going in and out of the victim machine. 

And this is why VPNs are important:)

