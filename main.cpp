#include <iostream>
#include "Disassembler.hpp"

int main(int argc, char *argv[])
{
   Disassembler disassembler;

   if(argc <= 1)
   {
      std::cout<<"Please input a filename\n";
   }
   else
   {
      disassembler.disassemble(argv[1]);
   }

   return 0;
}