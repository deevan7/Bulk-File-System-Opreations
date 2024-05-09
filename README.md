**README Bulk File System Opreations**
Deevankumar Gaddala

**File Handling:**
    - Create directories.
    - Appends text and numbers to text files and even numbers to binary files with 50 bytes after the number user enters and stops at max of 199 if it cross more than 50.
    - Deletes and directories.
    - Renames files and directories.
- **Error Handling:**
    - Returns system-defined error codes for potential issues.

**Available Operations**

**-c :** Creates a file.
**-d :** Deletes a file or empty directory.
**-r :** Renames a file or directory.
**-s :** appendtext appends text to the given file.
**-e :** appends the evennumbers if inputs numbver between 51-199 atmost 50 bytes.
**-l :** creates a log file and makes note of all the changes that we did and also it creates a new log file if its exixts then it will over write on it

**Examples**
compling the code with the given format
gcc my_bfm.c -o my_bfm


- Create a directory named "newdir":
`./my_bfm -c newdir`
- Create a directory named "newdir" and writes in the log file named logfile:
`./my_bfm -c newdir -l logfile`
-------------------------------------------------------------------------------------------------------

- Delete the file "newdir":
`./my_bfm -d newdir`
- Delete the file "newdir" and update the log file :
`./my_bfm -d newdir -l logfile`
-------------------------------------------------------------------------------------------------------

- Rename the file "old_name" to "new_name":
`./my_bfm -r  old_name new_name`
- Rename the file "old_name" to "new_name" and add the information to the log file:
`./my_bfm -r  old_name new_name -l logfile`
-------------------------------------------------------------------------------------------------------

- Appends the test given in "quotes" and numbers to the file:
`./my_bfm -s  filename "the text we want to add"`
- Appends the test given in "quotes" and numbers to the file and add to log file:
`./my_bfm -s  filename "the text we want to add" -l logfile`
(here we added "the text we want to add" is added to the file named new_name
-------------------------------------------------------------------------------------------------------

- Appends even numbers between 51-199 of 50 bytes:
`./my_bfm -e xyz.bin (even_numbers between 51 to 199)`

- Appends even numbers between 51-199 of 50 bytes and rewrites to the log file of what all digits are added to it:
`./my_bfm -e xyz.bin 51 -l logfile`
-------------------------------------------------------------------------------------------------------
