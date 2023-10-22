# Text Art
This program allows you to draw, edit, load, and save ASCII art in a canvas in the terminal.

## DISCLAIMER
**This program is only compatible with Windows machines since it utilizes Windows-specific header files.**

## NOTE
***The SavedFiles folder is necessary for loading and saving text art. The folder already has some art included, which can be loaded into the canvas and freely edited.***

## Menu Navigation
Navigate the menu by inputting the first letter of your desired feature. Prompts will follow to give further instruction once a choice has been made.

## Features
### Load
Gets a file name from the user. If the file can be opened for reading, the file's contents are loaded into the canvas. ***The file is a .txt file located in the SavedFiles folder.***

### Edit
Allows user to edit the canvas by moving the cursor around and entering characters. Editing continues until the ESC key is pressed. 

### Move
Shifts contents of the canvas by a specified number of rows and columns. ***Rows: positive numbers shift downward, negative numbers shift upward. Columns: positive numbers shift right, negative numbers shift left.***

### Replace
Replaces all instances of a character in the canvas. 

### Undo
Uses a copy of the canvas to revert the canvas back to its previous state (before the most recent change).

### Save
Gets a file name from the user. If the file can be opened for writing, the contents of the canvas are written into the file. ***The file is a .txt file located in the SavedFiles folder.***
