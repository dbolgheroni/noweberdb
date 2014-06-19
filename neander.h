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

#define INSTR 11

#define MEM_SIZE 256 /* bytes */
#define END int

/* Decimal Opcodes */
#define NOP 0
#define STA 16
#define LDA 32
#define ADD 48
#define OR  64
#define AND 80
#define NOT 96
#define JMP 128
#define JN  144
#define JZ  160
#define HLT 240

struct { /* n and z flags, respectively, negative and zero */
  unsigned n: 1;
  unsigned z: 1;
} flags;

union byte {
  struct { /* 8-bit computer */
    unsigned bit00: 1;
    unsigned bit01: 1;
    unsigned bit02: 1;
    unsigned bit03: 1;
    unsigned bit04: 1;
    unsigned bit05: 1;
    unsigned bit06: 1;
    unsigned bit07: 1;
  } _8bits;
  int _1byte; /* this is machine-dependent, but I'm looking for a way to */
	      /* make this a machine-independent code                    */
} ac, pc, memory[MEM_SIZE];


