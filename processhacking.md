### Process Hacking

Windows processes are easily hackable with native Windows tools. You can use ReadProcessMemory() and ReadProcessMemory() to read memory and modify it. 

All you need is a handle to the given process, and you are set.

### Creating the dummy program

Let us start by creating a simple dummy program that will act as our target. 

The following C++ code will result in a program that simply holds an integer value, an Account Balance, and displays it over and over upon pressing ENTER.

![Running](https://i.imgur.com/r2Ebyw6.png)

After compiling and running it we are greeted with a console window:

![Running](https://i.imgur.com/18Q3FRP.png)

Now let's open CheatEngine, attach to our dummyProgram.exe process, and search for our accountBalance value, which we know is 12345.

We found it at address ```0x00CFFAE8```

We then open ProcessHacker, and look up the PID of our program.

![Running](https://i.imgur.com/JMxteXj.png)

### Creating the attacker program

Now let's create another program, that will be the Attacker. 

The following C++ code gets access to our dummyProcess by its process ID, and then reads the value from the memory location into a variable.


![Running](https://i.imgur.com/7Sgcgko.png)

After running it, we can see that we successfully read the variable from a different process!

![Running](https://i.imgur.com/zVgAWhQ.png)

### But it doesn't end there

Let's now modify that variable from our attacker program.

We will need to write to the memory location that our accountBalance is stored in the dummyProgram.

We already know that it's stored at 0x00CFFAE8, so let's just write our desired balance there.

So let's add this code to do just that:

![Running](https://i.imgur.com/G89jCLq.png)

Now let's run them together:

![Running](https://i.imgur.com/c13N4WF.png)

Now let's press ENTER in both of them:

![Running](https://i.imgur.com/lmPavSx.png)

### We did it!

We modified our Account Balance through memory hacking. 

Why would Windows even have such a function, you may ask? Well, "originally it intended to allow debuggers to establish breakpoints and maintain instance data for other processes, but in practice, it allows both passive attacks such as scanning memory for high-entropy areas that constitute keys, and active attacks in which a target process' code or data is modified to provide supplemental functionality of benefit to a hostile process" (Cryptographic Security Architecture: Design and Verification
By Peter Gutmann)

Links:

https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory

https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory

https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess

https://github.com/processhacker/processhacker

https://github.com/cheat-engine/cheat-engine/


