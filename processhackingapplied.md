### Process Hacking Applied

We can use what we learned in [Process Hacking](https://prostotin.github.io/cybersecurity/processhacking) in a practical example to gain an advantage. 

Our test subject will be [CS:GO](https://en.wikipedia.org/wiki/Counter-Strike:_Global_Offensive). I used it because it's free, and is well known. 

![CSGO](https://i.imgur.com/FR1PDtb.png)

During tournaments, the game is shown to spectators with a special spectator view, which highlights player models through walls, to make it easier for spectators to follow the match. 

![CSGO](https://i.imgur.com/fZtSRbb.png)

So this gives us a hint that there is a function in the game that highlights the players when prompted. 

Upon doing some research, people who reverse engineered the game managed to find out that the Glowing effect is handled by the Glow Object Manager.

The Glow Object Manager holds the Glow Object Definition, which consists of the Entity pointer and its properties. 


![CSGO](https://i.imgur.com/5undaSl.png)


To get to that in memory, we have to jump through a few hoops, specifically:

clientDllBase -> dwEntityList -> entity -> m_iGlowIndex 

then from this, we go:

glowManager + m_iGlowIndex * 0x38 (size of the data).

At this point, ```(glowManager + (glowIndex * 0x38)```, we find the GlowObject, which we can then Read, modify, and then Write it back to the same address. 

So let's try this. 

First, we need to understand where CS:GO stores this data. It is stored in the client.dll module, and the offsets for the things we need can be found at [offsets](https://github.com/frk1/hazedumper/blob/master/csgo.hpp). We don't need all of them, so I will only choose the ones we actually care about. 

To find client.dll in memory, we need a function that loops through every module that was loaded and checks its name, and then returns its base address in case we find a match.

The following code will do just that: 

![CSGO](https://i.imgur.com/ysqHkyj.png)

Let's start with the same thing as before: we need the process ID and a handle to the process to be able to Read and Write memory.

```C++
HWND hwnd = FindWindowA(0, "Counter-Strike: Global Offensive"); //get hwnd of csgo 
GetWindowThreadProcessId(hwnd, &csgoProcessId); //get procid of csgo
std::cout << "CSGO PID: " << csgoProcessId << std::endl;

DWORD clientDllBase = GetModuleBaseExtern(csgoProcessId, "client.dll");
std::cout << "CLIENT BASE: " << clientDllBase << std::endl;

HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procId); //open handle 
  ```
 
 ![PID](https://i.imgur.com/7OpyI3I.png)
  
Let's cross reference this piece of code with what Process Hacker gives us: 

![PID](https://i.imgur.com/3m2dS7z.png)

It worked! Nice. Let's move on.

Now we need to grab some offsets.

Here is the list of the ones we need right now:


![PID](https://i.imgur.com/3BuTeEK.png)

That's it. Let's get Reading!

First we need to read the GlowOjectManager in.


![PID](https://i.imgur.com/fY4nOxI.png)

Since we need to read a struct, let's add a function template that will allow us to work with generic types.


![PID](https://i.imgur.com/eUKrb7o.png)



Then we need to loop through every entity, read its glowIndex, get its Glow Object Data, modify it inside our program, and then Write it back.


![GLOW](https://i.imgur.com/h7IPySc.png)

So let's see what this does!

![GLOW](https://i.imgur.com/QtpLLPA.png)

We can see that we successfully enabled the in-game glow with our nice looking purple color. It highlights enemies through walls, as well as makes them easier to see in plain sight. 

And this took less than a hundred lines of code! 

Although this might be tempting, please do not use this to gain an advantage over other players. Limit yourself to offline servers and bots. Also be aware that CS:GO has an anti-cheat system in place, and that you will be permanently banned if they detect that you're cheating. 

Refer to the [code](https://github.com/prostotin/cybersecurity/blob/master/Applied%20Process%20Hacking/glow.cpp) to learn more! :) 





