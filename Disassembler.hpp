#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <fstream>
#include <iostream>
#include <string>

class Disassembler
{
public:
   Disassembler();
   void disassemble(const std::string &fileName);
   ~Disassembler();
   static const int PROGRAM_START = 512;
private:
   void loadProgram(const std::string &fileName);
   char hexToChar(int h);
   unsigned char *buffer;
   unsigned int bufferLen;
};

#endif