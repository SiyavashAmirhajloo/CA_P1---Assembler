# CA_P1(Assembler)

## Project Description:

The "Assembler" project is a simple assembler program written in C that translates assembly code into machine code for a fictional computer architecture. It reads assembly code from a file (assprog.as) and produces machine code in another file (machprog.mc). The program uses a symbol table to resolve labels and generates the corresponding machine code for the supported assembly instructions.

## How to Use:

1. Clone the repository to your local machine:

   ```
   git clone https://github.com/your-username/assemble.git
   cd assemble
   ```

2. Build the program using a C compiler (e.g., GCC):

   ```
   gcc -o assemble assemble.c
   ```

3. Run the assembler with the input assembly program (assprog.as) and the output machine code file (machprog.mc):

   ```
   ./assemble assprog.as machprog.mc
   ```

4. After running the assembler, the machine code will be written to the "machprog.mc" file.

## Supported Instructions:

The assembler supports the following MIPS-inspired instructions:

- **R-Type** Instructions:
  - `add`
  - `sub`
  - `slt`
  - `or`
  - `nand`

- **I-Type** Instructions:
  - `addi`
  - `slti`
  - `ori`
  - `lui`
  - `lw`
  - `sw`
  - `beq`
  - `jalr`

- **J-Type** Instructions:
  - `j`

## Sample Assembly Program (assprog.as):

```
addi $t0, $t1, 5
lw $t2, 0($t0)
sw $t3, 4($t1)
j 100
halt
.fill 10
```

## Symbol Table:

The assembler uses a symbol table to keep track of labels and their corresponding memory addresses.

## Note:

This is a simplified assembler designed for educational purposes. It does not handle advanced features, optimizations, or error checking in real-world scenarios.

Feel free to explore and modify the code to suit your needs!

