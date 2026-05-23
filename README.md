- this shell will run:
    cd <dir> -changing directory
    clr -clear your screen
    dir - list directory content
    environ -show environment context
    echo - display text
    help - goes to help file
    pause - wait for user to enter
    quit - exit shell
- batch mode:
    takes commands from a file example : ./simpleshell batchfile
    everything will stop once user types quit
- External commands:
    commands that are not internal commands will run as external program
    the shell will fork and run the program as child process 
- Enviroment Variables:
    store all the things you need to know about the shell and 
    system shell= is the address of my simpleshell
    parent= set when external commands runs, it keeps where the shell is
- I/O redirection:
    . instead of the shell printing in the terminal you can put the output in a file 
    and instead of typing the input yourself you can get it from a file.
    . '<' this is for input file, reads input from a file.
    e.g echo hiiiiiii > hi.txt
    . '>' this is for putting the output in a file, creates a file to put the output inside
     e.g cat  > input.txt
    . '>>' appends to a file instead of overriding
     e.g echo hiiiiiii >> hi.txt
- Background execution:
    usually the shell waits for the command to terminate before asking you for another 
    command . adding & at the end lets another process run which will give u prompts while the other 
    process is doing another task.
    .sleep 5 - shell sleeps for 5 seconds and will stop until 5 seconds is over
    . sleep 5 & - while main one is sleeping another process is working and gives you a prompt to enter command
    