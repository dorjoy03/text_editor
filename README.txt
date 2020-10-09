This is a personal project i worked for a couple of days from scratch. I tried
to build a somewhat emacs-like text editor for use in terminals. It's very
basic, just supports a few keyboard controls and that's it. It is written in C.
This README file is being written using this program.

author: Dorjoy Chowdhury
email: dorjoychy111@gmail.com

* How to compile?
--> Copy the source code folder anywhere in you want and move to that directory.
    You will have to have ncurses library installed.Type the following command:

    gcc -O2 struct_data.c char_check.c key_functions.c main.c -o main -lcurses

* How to run?
--> Type the program path in terminal and the filename you want to edit or create.
    Example: ./main filename
    You can type ./main -help for help.

* Controls
--> Below C stands for the control key on keyboard.

    C-f --> Move forward one character
    C-b --> Move backward one character
    C-n --> Move to the start of the next line
    C-p --> Move to the start of the previous line
    C-a --> Move to the start of the current line
    C-e --> Move to the end of the current line
    C-s --> Save to file
    C-k --> Exit
    
* It doesn't support terminal window size change. But it should be fairly easy
  to add this feature with a SIGWINCH signal handler.

* In my terminal TAB takes cursor to the multiple of 8 positions. So i build my
  data structure accordingly. 

* I have only compiled and run the program on ubuntu 20.04 and the default
  terminal was used.
