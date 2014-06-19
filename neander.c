/*
 * Copyright (c) 2003, Daniel Bolgheroni. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY DANIEL BOLGHERONI ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DANIEL BOLGHERONI OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Computer Description
 * ======== ===========
 *
 * Instructions
 * ------------
 *
 * o Code: 0000, Instruction: NOP    , Description: no operation
 * o Code: 0001, Instruction: STA end, Description: MEM(end) <- AC
 * o Code: 0010, Instruction: LDA end, Description: AC <- MEM(end)
 * o Code: 0011, Instruction: ADD end, Description: AC <- MEM(end) + AC
 * o Code: 0100, Instruction: OR  end, Description: AC <- MEM(end) OR AC
 * o Code: 0101, Instruction: AND end, Description: AC <- MEM(end) AND AC
 * o Code: 0110, Instruction: NOT    , Description: AC <- NOT AC
 * o Code: 1000, Instruction: JMP    , Description: PC <- end
 * o Code: 1001, Instruction: JN  end, Description: IF N=1 then PC <- end
 * o Code: 1010, Instruction: JZ  end, Description: IF Z=1 then PC <- end
 * o Code: 1111, Instruction: HLT    , Description: halt
 */

#include <stdio.h>
#include "neander.h"

/* Instructions */
void nop(void);
void sta(void);
void lda(void);
void add(void);
void or(void);
void and(void);
void not(void);
void jmp(void);
void jn(void);
void jz(void);
void hlt(void);

/* Decimal Table */
struct instr {
  int opcode;
  void (*mnemonic)();
} neander[] = {
  0  , nop,
  16 , sta,
  32 , lda,
  48 , add,
  64 , or , 
  80 , and,
  96 , not,
  128, jmp,
  144, jn ,
  160, jz , 
  240, hlt
};

/* Run Instructions                                              */
/* declared as 'extern' because need to be accessed by debugger, */
/* which is where the user interacts with the computer  */ 
void step_instr(void);
void run_instr(int bp);

/* used to check and change flags */
void change_flags(void);

/* Here is the heart of the simulator, how computer run instructions... */
void step_instr(void) {
  int i;

  for (i = 0; i < INSTR; i++)
    if (memory[pc._1byte]._1byte == neander[i].opcode)
      (*neander[i].mnemonic)();
}

void run_instr(int bp) {
  int i;

  while(memory[pc._1byte]._1byte != HLT) { /* HLT */
    if(pc._1byte == bp) {
      printf("Stopped at %i. Press any key to continue.\n");
      getchar();
    }

    for(i = 0; i < INSTR; i++)
      if(memory[pc._1byte]._1byte == neander[i].opcode)
        (*neander[i].mnemonic)();
  }
}

void change_flags(void) {
  if (ac._1byte < 0)
    flags.n = 1;

  if (ac._1byte == 0)
    flags.z = 1;
}

/* here begins the instructions itself */
void nop(void) {
  pc._1byte++;
}

void sta(void) {
  memory[memory[pc._1byte + 1]._1byte]._1byte = ac._1byte;
  pc._1byte += 2;
}

void lda(void) {
  ac._1byte = memory[memory[pc._1byte + 1]._1byte]._1byte;
  pc._1byte += 2;

  change_flags();
}

void add(void) {
  ac._1byte = ac._1byte + memory[memory[pc._1byte + 1]._1byte]._1byte;
  pc._1byte += 2;

  change_flags();
}

void or(void) {
  ac._1byte = ac._1byte | memory[memory[pc._1byte + 1]._1byte]._1byte;
  pc._1byte += 2;

  change_flags();
}

void and(void) {
  ac._1byte = ac._1byte & memory[memory[pc._1byte + 1]._1byte]._1byte;
  pc._1byte += 2;

  change_flags();
}

void not(void) {
  ac._1byte = (~ac._1byte);
  pc._1byte++;

  change_flags();
}

void jmp(void) {
  pc._1byte = memory[pc._1byte + 1]._1byte;
}

void jn(void) {
  if (ac._1byte < 0) {
    pc._1byte = memory[pc._1byte + 1]._1byte;
  } else {
    pc._1byte += 2;
  }
}

void jz(void) {
  if (ac._1byte == 0) pc._1byte = memory[pc._1byte]._1byte;
  else pc._1byte += 2;
}

/* symbolic */
void hlt(void) {

}

