# Legv8Sim

# How to Run
When the program is first started, it will prompt the user to enter the name of the file (which should be in the same directory as the program) that contains the instructions to be run.
After the program reads the file, the user will be prompted to either enter 1 to run through all instructions at once, or 2 to run each instruction step by step as queued by the user.
If the user chooses step mode, the user will be prompted at each instruction to either enter 1 to execute the next instruction, 2 to show specific contents of memory, or 3 to terminate execution without continuing to the next instruction.
If the user chooses to show memory, the program will prompt them to specify wether they would like to see Register or Memory, and what specific location in either.
Upon completion, the contents of both Register and Memory will be written to output.txt. All Register values will be written, but due to the large size of Memory, only the bytes that were written to will be shown in the output file (therefore if no Memory was written to, none will be shown).
All data values for both Register and Memory are displayed in decimal form.
