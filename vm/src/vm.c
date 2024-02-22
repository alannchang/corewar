/*
General virtual machine strategy (work in progress)
- read .cor files
    - parse the header
    - parse champion name
    - parse champion instructions
- Load programs. Each program has
    - register section
    - counter, this denotes the location of
    - carry flag
- Create arena
    - array of structs. Each structs contain
        -tbd
- implement logic of instructions
    - make sure instructions are correct (may be low priority)
    - 
*/

#include "../inc/vm.h"