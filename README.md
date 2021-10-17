# Operating_systems
This project broadly aims to give an understanding of what a modern operating system is, and the services it provides. It also discusses some basic issues in operating systems and provides solutions. Topics include multiprogramming, process management, memory management, and file systems.

## List Implementation (UNIX)

For this project, I have implemented the List abstract data type. The list data structure is widely used throughout operating system programming. This project helped my list skills onto UNIX and into C programming.

I have created the user-defined type List. An instance of this type refers to a particular list and will be an argument to most of your list manipulation routines. As with all code that is written for operating systems, the goal here is efficiency.

## Make file
This project contains a makefile :
###
      “make”: builds the program
      “make clean”: erases all build products 

## IPC and Concurrency

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
