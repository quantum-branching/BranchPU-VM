### What is this?

This is a set of virtual machines for some instruction architectures that I have built, collectively know as BranchPU ISAs.

Since there is more than one architecture, I have developed a naming scheme as follows (where terminating `_0` is dropped):

`"Q" + dec2hex(floor(instruction_length / 8)) + dec2hex(argument_length) + dec2hex(design) + "_" + String(iteration)`

Currently Supports:
* Q240
* Q242
* Q280

Currently Unsupported:
* Q241
* Q281
* Q340

### Obtaining Executable

Download pre-compiled Executable (Windows or Linux)
* [Navigate to Releases](https://github.com/quantum-branching/BranchPU-VM/releases)
* Download appropriate Executable (labeled \<OS\>\_\<Architecture\>\_BranchPU\_VM)

Compile from Source (Any platform)
* Download Source
    * From Release
    * From Branch
* Download a C compiler (If you don't already have it)
  * [GCC (GNU Compiler Collection)](https://gcc.gnu.org/install/)
  * [LLVM/Clang](https://releases.llvm.org/)
  * [TCC (Tiny C Compiler)](https://github.com/cnlohr/tinycc-win64-installer)
* Run `cd <Source Directory>`
* Compile with `Build/unix.sh` on Linux or Mac

### Opening Executable Files
You can open a .q***x file in the BPU VM by passing that file as an argument
* Drop file on executable
* Open file with BranchPU-VM
* Command-line `[BranchPU-VM filepath] [Q***X file]`
