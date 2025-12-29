# clipz
A REALLY simple clip manager. 

I needed something to save multiple text lines to multiple "clip locations" and than paste them, one at a time and just with shortcuts. 
I used windows 11 advanced copy/paste (Win+V) or Ditto, but passing through UI was tedious during heavy data entry sessions. 
Clipz wants to simplify as much as possible this horrendous tasks.

It has 4 modes (+ quit with Ctrl+Alt+Q) and whenever text is saved, it is also printed on stdout with format [Key] = "[Value]".

[COPY MODE]
As of right now you can copy up to 9 text strings associated to numbers on keyboard (1-9).
In future could be implemented more slots, but if you are doing data entry it can be difficult to remember the whereabouts of more than 9 entries, so for me it was a good compromise.

1) Select text to copy
2) Ctrl+Alt+A -> Copy mode activated
3) Ctrl+(1-9) -> Copy text to that number and Copy mode deactivated
4) Ctrl+(1-9) -> Paste text saved to that number

[STACK MODE]
Rarely used by me, but i thought copying multiple things from different parts of a doc and then pasting them in order could be useful.

1) Select text to copy
2) Ctrl+Alt+0 -> Copy text to 0
3) You can repeat 1 and 2 N times to copy text to 0 (this will be saved in LIFO mode) 
4) Ctrl+0 -> Paste text saved in LIFO mode

[TEMPLATE MODE]
It needs a string with '%' placeholders and than you can copy/paste other text replacing the placeholder. Ex: string 1 = "Hello, %!"; string 2 = "World"; paste -> "Hello, World!".
A little tricky but it was useful and can be refined for sure.

1) Ctrl+Alt+T -> Template mode on
2) Select string1 to copy
3) Ctrl+Alt+(1-9) -> copy string1 to (1-9)
4) Ctrl+Alt+A -> Copy mode on
5) Select string2 to copy
6) Ctrl+Alt+NumPad(1-9) -> copy to Numpad(1-9)
7) Ctrl+Alt+T -> Paste mode on
8) Ctrl+(1-9) Ctrl+NumPad(1-9) -> Paste combined strings

[PRINT MODE]
Cleans the output and shows only current saved text strings.

1)Ctrl+Alt+P -> refresh console with latest saved text
