1. **Start of Execution:**
    
    - SP = 0C001,000
    - PC = 00000,FFC
    - All page table entries are initially marked non-present.
2. **Execution Steps:**
    
    **Step 1: Instruction Fetch at 00000,FFC**
    
    - attempt(instruction fetch at 00000,FFC)
    - fault(00000,FFC, instruction load) – Page fault occurs because the page is not present.
    - page allocate = 00001 – Allocate a physical page for the program code.
    - 00000[0].page = 00001 – Set the 0th entry of the page directory to point to the allocated page.
    - read_block(/bin/program, 0, 00001) – Read the first block of the program from disk into physical page 00001.
    - return_from_fault
    
    **Step 2: Repeat Fetch after Page Fault Resolution**
    
    - attempt(instruction fetch at 00000,FFC)
    - success
    
    **Step 3: Execute PUSH #10**
    
    - attempt(PUSH #10)
    - SP = 0C000,FFC (decrement SP by 4 before use)
    - fault(0C000,FFC, data store) – Page fault occurs because the stack page is not present.
    - page allocate = 00002 – Allocate a physical page for the stack.
    - 00000[C00].page = 00002 – Set the C00th entry of the page directory to point to the allocated stack page.
    - read_block(unknown, 0C000, 00002) – Assume we read the corresponding stack page from the backing file.
    - return_from_fault
    
    **Step 4: Repeat PUSH after Page Fault Resolution**
    
    - attempt(PUSH #10)
    - store(0C000,FFC) – Store the constant #10 on the stack.
    - success
    
    **Step 5: Instruction Fetch at 00001,000**
    
    - attempt(instruction fetch at 00001,000)
    - fault(00001,000, instruction load) – Page fault occurs because the page is not present.
    - page allocate = 00003 – Allocate a physical page for the continuation of the program code.
    - 00000[1].page = 00003 – Set the 1st entry of the page directory to point to the allocated page.
    - read_block(/bin/program, 1, 00003) – Read the next block of the program from disk into physical page 00003.
    - return_from_fault
    
    **Step 6: Repeat Fetch after Page Fault Resolution**
    
    - attempt(instruction fetch at 00001,000)
    - success
    
    **Step 7: Execute CALL 2,000**
    
    - attempt(CALL 2,000)
    - SP = 0C000,FF8 (decrement SP by 4 before use)
    - store(0C000,FF8) – Store return address on the stack.
    - PC = 00002,000 – Set PC to the address of the subroutine to call.
    - success
    
    **Step 8: Instruction Fetch at 00002,000**
    
    - attempt(instruction fetch at 00002,000)
    - fault(00002,000, instruction load) – Page fault occurs because the page is not present.
    - page allocate = 00004 – Allocate a physical page for the subroutine code.
    - 00000[2].page = 00004 – Set the 2nd entry of the page directory to point to the allocated page.
    - read_block(/bin/program, 2, 00004) – Read the block of the program that includes the subroutine from disk into physical page 00004.
    - return_from_fault
    
    **Step 9: Repeat Fetch after Page Fault Resolution**
    
    - attempt(instruction fetch at 00002,000)
    - success
    
    *_Step 10: Execute MOV EAX → _(10,000)__
    
    - attempt(MOV EAX → *(10,000))
    - fault(0100A,000, data load) – Page fault occurs because the data page is not present.
    - page allocate = 00005 – Allocate a physical page for the data.
    - 00000[100A].page = 00005 – Set the 100Ath entry of the page directory to point to the allocated data page.
    - read_block(unknown, 0100A, 00005) – Read the corresponding data page from the backing file.
    - return_from_fault
    
    **Step 11: Repeat MOV after Page Fault Resolution**
    
    - attempt(MOV EAX → *(10,000))
    - load(0100A,000) – Load data from the memory address into EAX.
    - success
    
    **Step 12: Instruction Fetch at 00002,004**
    
    - attempt(instruction fetch at 00002,004)
    - success (Assuming the current page contains this instruction as well)
    
    **Step 13: Execute HALT**
    
    - attempt(HALT)
    - success – Execution is halted.