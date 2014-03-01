#include "Disassembler.hpp"

Disassembler::Disassembler()
{
   bufferLen = 0;
}

void Disassembler::disassemble(const std::string &fileName)
{
   // Load the program into the buffer first
   loadProgram(fileName);
   
   // Now open up another file to save the disassembly into
   std::string outputName = fileName + ".txt";
   std::ofstream output(outputName.c_str());

   if(output)
   {
      for(unsigned int i = 0; i < bufferLen; i += 2)
      {
         unsigned short opcode = (buffer[i] << 8) | (buffer[i + 1]);
         bool unknownOpcode = false;
         output<<"mem"<<i<<":   ";
         switch(opcode & 0xF000)
         {
         case 0x0000:
            {
               switch(opcode & 0x00FF)
               {
               case 0x00E0:      // 0x00E0: clr
                  {
                     output<<"clr";
                     break;
                  }
               case 0x000E:      // 0x00EE: ret
                  {
                     output<<"ret";
                     break;
                  }
               default:
                  {
                     unknownOpcode = true;
                  }
               }
               break;
            }
         case 0x1000:   // 0x1NNN: jmp memnnn
            {
               int num = (opcode & 0x0FFF) - PROGRAM_START;
               output<<"jmp mem"<<num;
               break;
            }
         case 0x2000:   // 0x2NNN: call memnnn
            {
               int num = (opcode & 0x0FFF) - PROGRAM_START;
               output<<"call mem"<<num;
               break;
            }
         case 0x3000:   // 0x3XNN: se vx, nn
            {
               int reg = (opcode & 0x0F00) >> 8;
               int num = (opcode & 0x00FF);
               output<<"se v"<<hexToChar(reg)<<", ";
               if(num <= 0xF)
               {
                  output<<"0";
               }
               output<<std::hex<<num;
               break;
            }
         case 0x4000:   // 0x4XNN: sne vx, nn
            {
               int reg = (opcode & 0x0F00) >> 8;
               int num = (opcode & 0x00FF);
               output<<"sne v"<<hexToChar(reg)<<", ";
               if(num <= 0xF)
               {
                  output<<"0";
               }
               output<<std::hex<<num;
               break;
            }
         case 0x5000:   // 0x5XY0: se vx, vy
            {
               int reg1 = (opcode & 0x0F00) >> 8;
               int reg2 = (opcode & 0x00F0) >> 4;
               output<<"se v"<<hexToChar(reg1)<<", v"<<hexToChar(reg2);
               break;
            }
         case 0x6000:   // 0x6XNN: set vx, nn
            {
               int reg = (opcode & 0x0F00) >> 8;
               int num = (opcode & 0x00FF);
               output<<"set v"<<hexToChar(reg)<<", ";
               if(num <= 0xF)
               {
                  output<<"0";
               }
               output<<std::hex<<num;
               break;
            }
         case 0x7000:   // 0x7XNN: add vx, nn
            {
               int reg = (opcode & 0x0F00) >> 8;
               int num = (opcode & 0x00FF);
               output<<"add v"<<hexToChar(reg)<<", ";
               if(num <= 0xF)
               {
                  output<<"0";
               }
               output<<std::hex<<num;
               break;
            }
         case 0x8000:
            {
               switch(opcode & 0x000F)
               {
               case 0x0000:      // 0x8XY0: set vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"set v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0001:      // 0x8XY1: or vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"or v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0002:      // 0x8XY2: and vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"and v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0003:      // 0x8XY3: xor vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"xor v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0004:      // 0x8XY4: add vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"add v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0005:      // 0x8XY5: sub vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"sub v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0006:      // 0x8XY6: shr vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"shr v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x0007:      // 0x8XY7: subx vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"subx v"<<reg1<<", v"<<reg2;
                     break;
                  }
               case 0x000E:      // 0x8XYE: shl vx, vy
                  {
                     int reg1 = (opcode & 0x0F00) >> 8;
                     int reg2 = (opcode & 0x00F0) >> 4;
                     output<<"shl v"<<reg1<<", v"<<reg2;
                     break;
                  }
               default:
                  {
                     unknownOpcode = true;
                  }
               }
               break;
            }
         case 0x9000:      // 0x9XY0: sne vx, vy
            {
               int reg1 = (opcode & 0x0F00) >> 8;
               int reg2 = (opcode & 0x00F0) >> 4;
               output<<"sne v"<<reg1<<", v"<<reg2;
               break;
            }
         case 0xA000:      // 0xANNN: ind memnnn
            {
               int num = (opcode & 0x0FFF) - PROGRAM_START;
               output<<"ind mem"<<num;
               break;
            }
         case 0xB000:      // 0xBNNN: jmp0 memnnn
            {
               int num = (opcode & 0x0FFF) - PROGRAM_START;
               output<<"jmp0 mem"<<num;
               break;
            }
         case 0xC000:      // 0xCXNN: rnd vx, nn
            {
               int reg = (opcode & 0x0F00) >> 8;
               int num = (opcode & 0x00FF);
               output<<"rnd v"<<hexToChar(reg)<<", ";
               if(num <= 0xF)
               {
                  output<<"0";
               }
               output<<std::hex<<num;
               break;
            }
         case 0xD000:      // 0xDXYN: draw vx, vy, n
            {
               int reg1 = (opcode & 0x0F00) >> 8;
               int reg2 = (opcode & 0x00F0) >> 4;
               int num = (opcode & 0x000F);
               output<<"draw v"<<hexToChar(reg1)<<", v"<<hexToChar(reg2)<<", "<<std::hex<<num;
               break;
            }
         case 0xE000:
            {
               switch(opcode & 0x00FF)
               {
               case 0x009E:      // 0xEX9E: kse vx
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"kse v"<<hexToChar(reg);
                     break;
                  }
               case 0x00A1:      // 0xEXA1: ksn vx
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"ksn v"<<hexToChar(reg);
                     break;
                  }
               default:
                  {
                     unknownOpcode = true;
                  }
               }
               break;
            }
         case 0xF000:
            {
               switch(opcode & 0x00FF)
               {
               case 0x0007:      // 0xFX07: del vx
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"del v"<<hexToChar(reg);
                     break;
                  }
               case 0x000A:      // 0xFX0A: wkp vx
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"wkp v"<<hexToChar(reg);
                     break;
                  }
               case 0x0015:      // 0xFX15:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"setd v"<<hexToChar(reg);
                     break;
                  }
               case 0x0018:      // 0xFX18:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"sets v"<<hexToChar(reg);
                     break;
                  }
               case 0x001E:      // 0xFX1E:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"adi v"<<hexToChar(reg);
                     break;
                  }
               case 0x0029:      // 0xFX29:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"spr v"<<hexToChar(reg);
                     break;
                  }
               case 0x0033:      // 0xFX33:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"sbi v"<<hexToChar(reg);
                     break;
                  }
               case 0x0055:      // 0xFX55:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"sto v"<<hexToChar(reg);
                     break;
                  }
               case 0x0065:      // 0xFX65:
                  {
                     int reg = (opcode & 0x0F00) >> 8;
                     output<<"fil v"<<hexToChar(reg);
                     break;
                  }
               }
               break;
            }
         default:
            {
               unknownOpcode = true;
            }
         }
         if(unknownOpcode)
         {
            output<<"dw ";
            if(opcode <= 0xF00)
            {
               output<<"0";
            }
            if(opcode <= 0xF0)
            {
               output<<"0";
            }
            if(opcode <= 0xF)
            {
               output<<"0";
            }
            output<<std::hex<<opcode;
         }
         output<<std::endl;
      }
      output.close();
      std::cout<<"Disassembled to "<<outputName<<std::endl;
   }
   else
   {
      std::cout<<"Error reading file "<<outputName<<" for output"<<std::endl;
   }
}

Disassembler::~Disassembler()
{
   if(bufferLen != 0)
   {
      delete[] buffer;
   }
}

void Disassembler::loadProgram(const std::string &fileName)
{
   std::ifstream input(fileName.c_str(), std::ifstream::binary);

   if(input)
   {
      // Get the length of the file
      input.seekg(0, input.end);
      bufferLen = input.tellg();
      input.seekg(0, input.beg);

      // Make our buffer
      buffer = new unsigned char[bufferLen];

      // Read from the file
      input.read((char*)buffer, bufferLen);

      if(!input)
      {
         std::cout<<"Error reading file, only "<<input.gcount()<<" could be read"<<std::endl;
      }

      input.close();
   }
   else
   {
      std::cout<<"Error reading file "<<fileName<<std::endl;
   }
}


char Disassembler::hexToChar(int h)
{
   if((h < 0) || (h > 0xF))
   {
      return '?';
   }
   else if(h < 10)
   {
      return h + '0';
   }
   else
   {
      return h + ('a' - 0xA);
   }
}
