/*
 * Copyright (c) 2003, Daniel Bolgheroni.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * o  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * o  All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *
 * o  Neither the name of Daniel Bolgheroni nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define COMPUTER "noweber"
#define VERSION "0.1"
#define PROMPT "noweberdb> "
#define CMD_NR 8 /* number of commands */
#define CMD_POS 0
#define NOT_FOUND 999

const char *help_msg = "\
Help                                                              \n\
------------------------------------------------------------------\n\
o (n)eander   : shows Neander information                         \n\
o (p)rint [ac|pc|[mem mem_addr]]                                  \n\
  o ac: print accumulator (AC)                                    \n\
  o pc: print program counter (PC)                                \n\
  o mem mem_addr: print memory contents                           \n\
  Example: print mem 64                                           \n\
o (e)dit [ac|pc|[mem mem_addr mem_cont]]                          \n\
  o ac: edit accumulator (AC)                                     \n\
  o pc: edit program counter (PC)                                 \n\
  o mem mem_addr mem_cont: edit memory contents                   \n\
  Example: edit mem 64 7                                          \n\
o (b)reakpoint mem_addr: adds a breakpoint in mem_addr            \n\
o (s)tep: run the instruction at memory[PC] until next instruction\n\
o (r)un: run the instruction at memory[PC] until HLT              \n\
o (q)uit: (...)                                                   \n\
------------------------------------------------------------------\n\
Neander is the only computer which can be simulated until now.    \n";

const char *neander_info = "\
Instructions\n\
+-----------+-------------+-----------------------+\n\
| Code      | Instruction | Description           |\n\
+-----------+-------------+-----------------------+\n\
| 0000 XXXX | NOP         | no operation          |\n\
| 0001 XXXX | STA end     | MEM(end) <- AC        |\n\
| 0010 XXXX | LDA end     | AC <- MEM(end)        |\n\
| 0011 XXXX | ADD end     | AC <- MEM(end) + AC   |\n\
| 0100 XXXX | OR  end     | AC <- MEM(end) OR AC  |\n\
| 0101 XXXX | AND end     | AC <- MEM(end) AND AC |\n\
| 0110 XXXX | NOT         | AC <- NOT AC          |\n\
| 1000 XXXX | JMP         | PC <- end             |\n\
| 1001 XXXX | JN  end     | IF N=1 then PC <- end |\n\
| 1010 XXXX | JZ  end     | IF Z=1 then PC <- end |\n\
| 1111 XXXX | HLT         | halt                  |\n\
+-----------+-------------+-----------------------+\n\
| Memory Size: 256 bytes [0-ff]                   |\n\
+-------------------------------------------------+\n";

const char *error[] = {
  "Invalid command. Type \"help\" for details.\n",
  "Invalid or missing argument. Type \"help\" for details.\n",
};

void step_instr(void);
void run_instr(int bp);


