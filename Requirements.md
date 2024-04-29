# Requirements

---
## FR-1 - Character Input
### User presses a printable character on the keyboard
### Window outputs requested character
---
## FR-2 - Space Key Press
### User presses spacebar on the keyboard
### Window outputs single space
---
## FR-3 - Tab Key Press
### User presses tab on the keyboard
### Window outputs 4 spaces
---
## FR-3 - Backspace Key Press
### User presses backspace on the keyboard
### Window removes character before the cursor, only removing characters which are from the user input/text file, otherwise produce error beep
- Note: Consider future support to allow backspacing into the previous line
---
## FR-4 - Arrow Left Press
### User presses left arrow on the keyboard
### Window moves cursor left one space if there are any characters to the left
---
## FR-5 - Arrow Right Press
### User presses right arrow on the keyboard
### Window moves cursor right one space if there are any characters to the right
---
## FR-6 - Arrow Up Press
### User presses up arrow on the keyboard
### Window moves cursor up one space if there are any lines above the current line. If the line above is outside of scrolling region, window scrolls up one line and cursor stays on current line
---
## FR-7 - Arrow Down Press
### User presses down arrow on the keyboard
### Window moves cursor down one space if there are any lines below the current line. If the line below is outside of scrolling region, window scrolls down one line and cursor stays on current line
---
## FR-8 - Enter Key Press
### User presses enter key
### Window prints new line underneath, with the number of the line printed on the left. If cursor is not on the last line of the file, all subsequent lines underneath the current line are shifted down underneath the new line being created. If the cursor is between characters, the characters to the right of the cursor are moved to the new line underneath the current line.
---
## FR-9 - Program Exit
### User presses F9 on the keyboard
### Program exits if file is saved. If it is not saved, program will prompt user to save first
---
## FR-10 - Save File
### User presses F8 on the keyboard
### Window prompts user for file name. If file already exists or has been saved previously, autofill the file's name while allowing user to change the name if they desire; returning the file name will prompt application to save data to said file and return control to the text editor
---
...
...
