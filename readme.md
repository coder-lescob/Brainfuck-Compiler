# Brainfuck compiler

Yes this is a brainfuck compiler. It compiles brainfuck to assembly using custom pipline (Very simple for bf). For now it only compiles on linux because I'm not a big fan of the windows kernel api.

## What is brainfuck

Brainfuck is an *isoteric programming language* which basically means that it is hard to do anything usefull with it but it is in theory possible. Brainfuck works on a 1D tape of bytes that can be increamented with **+** and decremented with **-**. Then the instructions **>** and **<** moves the tape reader around the tape left and right. Finally in between **[** and **]** code is repeated until the current cell has a value of *0*. You may recall that this kind of layout is very similar to a **turing machine**, the theoretical model of a computer. And thus **brainfuck** is **turing complete** which means that it can execute any algrithm of the world, with the added bonus to be harder than **C++**.

## run the current version

Download the latest release of the compiler. Then run it on your input file like so:

```bash
bf64 <input_file> -o <output_file>
```

Then when it is compiled the file output_file is the assembly representation of your bf code, so you need to assemble it with:

```bash
nasm -felf64 <output_file> -o <object_file>
```

Then link it with

```bash
ld <object_file> -o <binary_file>
```

Then you can run it with:

``` bash
./<binary_file>
```
