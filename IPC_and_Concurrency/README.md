# IPC and Concurrency

For this project, I have created a simple "chat"-like facility that enables someone at one terminal (or Xterm) to communicate with someone at another terminal. The interface will not be pretty, but it will be functional. This facility will be called "s- talk" for simple-talk.

To initiate an s-talk session two users will first agree on two things: 
 - the machine that each will be running on
 - the port number (explained later) each will use
Say that Fred and Barney want to talk. Fred is on machine "csil-cpu1" and will use port number 6060. Barney is on machine "csil-cpu3" and will use port number 6001.
### To initiate s-talk, Fred must type:
      s-talk 6060 csil-cpu3 6001
### And Barney must type:
      s-talk 6001 csil-cpu1 6060.
### So, the general format is:
      s-talk [my port number] [remote machine name] [remote port number]
The result will be that every line typed at each terminal will appear on BOTH terminals: it will appear character-by-character on the sender’s terminal as they type it, and it will appear on the receiver’s terminal once the sender presses enter.
An s-talk session is terminated when either user enters the complete line of text which is just one ‘!’ and presses enter.

## Make file
This project contains a makefile :
###
      stalk : begin s-talk
      clean : erases all build products 
