![6502 Emulator in action](./emu.png)
# Simple 6502 Emulator
- Written in C++  
# Status  
It probably works? Haven't tested anything yet...  
Currently working with Curses UI  
# Building
* Clone this directory  
* Go to /build folder  
  ```cd {PATH_TO_CLONED_DIRECTORY}/6502Emu/build```
* Set srcPath directory (For Makefile)  
  ```export srcPath='{PATH_TO_SRC_DIRECTORY}'``` 
* Use GNU Make to build
  ```make -f "../Makefile" all```
