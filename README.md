# mp8085
 An emulator for the 8085 microprocessor. Inspired by javidx9's [OneLoneCoder] NES emulator. https://github.com/OneLoneCoder/olcNES

 Using it is fairly simple, assign your opcodes to the ``ss`` variable in the ``test.cpp`` file (line #17)

 ## To Do:

 * Implement ``HLT`` instruction.
    * Possibly add a ``start()`` method to start the clock on its own, instead of needing an external clock call
 * Remove dependency on ``maxCycles`` in ``test.cpp``
 * ~Better~ Some form of GUI
 * Better hex code (opcodes) input
 * 8085 mnemonic-to-opcode converter?
 * Improve debugging methods
 
