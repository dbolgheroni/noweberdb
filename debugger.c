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
 * This the main file, since it controls almost everything.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <histedit.h>

#include "debugger.h"
#include "neander.h" /* need to change this for some others CPUs */

/* global variables */
int bp;

/* function prototypes for debugger */
char *prompt_f(void);

int help(void);
int hardware(void);
int print(const char *args[]);
int edit(const char *args[]);
int breakpoint(const char *args[]);
int step(void);
int run(void);
int quit(void);

int main(int argc, char *argv[]) {
  int i, found, count, args_nr;
  const char *cmd, **args;
  char *no_nl;
  EditLine *el;
  History *db_hist;
  HistEvent hist_ev;
  Tokenizer *tok;

  const struct commands {
    const char *name;
    const char *sname;
    int (*func)();
  } db_cmds[] = {
    { "h", "help"      , help         },
    { "n", "neander"   , hardware     },
    { "p", "print"     , print        },
    { "e", "edit"      , edit         },
    { "b", "breakpoint", breakpoint   },
    { "s", "step"      , step         },
    { "r", "run"       , run          },
    { "q", "quit"      , quit         }
  };

  /* configuring libedit */
  el = el_init(argv[0], stdin, stdout, stderr);
  el_set(el, EL_PROMPT, &prompt_f);
  el_set(el, EL_EDITOR, "emacs");

  /* configuring libedit history */
  db_hist = history_init();
  history(db_hist, &hist_ev, H_SETSIZE, 64);
  el_set(el, EL_HIST, history, db_hist);
  
  printf(COMPUTER "db " VERSION "\n");
  printf("Type \"help\" to see available commands.\n\n");

  for(;;) {
    cmd = el_gets(el, &count);
    
    /* only non-empty commands are stored in history */
    if(count > 1) {
      history(db_hist, &hist_ev, H_ENTER, cmd);

      /* using libedit to do tokenization functions */
      tok = tok_init(NULL);
      tok_str(tok, cmd, &args_nr, &args);
      tok_reset(tok);
    } else
      continue;

    found = 1;
 
    /* checks the command syntax */
    for(i = 0; i < CMD_NR; i++) {
      if(!strcmp(db_cmds[i].name, args[0]) ||
	 !strcmp(db_cmds[i].sname, args[0])) {
        found = (*db_cmds[i].func)(args);
        break;
      }
    }

    if(found == 1)
      printf(error[0]);
  }

  history_end(db_hist);
  el_end(el);

  return 0;
}

/* function to return the prompt */
/* that's the way libedit works  */
char *prompt_f(void) {
  return PROMPT;
}

int help(void) {
  printf(help_msg);
  return 0;
}

int hardware(void) {
  printf(neander_info);
  return 0;
}

int print(const char *args[]) { 
  int i, arg_code, mem_addr, found = 1;
  const int hex;
  const char *print_args[] = {
    "ac",
    "pc",
    "mem"
  };

  if(args[1])
    for(i = 0; i < 3; i++) {
      if(!strcmp(args[1], print_args[i])) {
	arg_code = i;
	found = 0;
	break;
      }
    }

  if(found == 1)
    arg_code = NOT_FOUND;

  switch(arg_code) {
    case 0:
      /* need to change this printf for other computers, */
      /* just for better visualization                   */
      printf(" AC ===> b: %d%d%d%d%d%d%d%d, d: %4d, h: %#10X, N: %d, Z: %d\n",
	     ac._8bits.bit07,
	     ac._8bits.bit06,
	     ac._8bits.bit05,
	     ac._8bits.bit04,
	     ac._8bits.bit03,
	     ac._8bits.bit02,
	     ac._8bits.bit01,
	     ac._8bits.bit00,
	     ac._1byte,
	     ac._1byte,
	     flags.n,
	     flags.z
	     );      
      break;

    case 1:
      /* need to change this printf for other computers, */
      /* just for better visualization                   */
      printf(" PC ===> b: %d%d%d%d%d%d%d%d, d: %4d, h: %#10X, N: %d, Z: %d\n",
	     pc._8bits.bit07,
	     pc._8bits.bit06,
	     pc._8bits.bit05,
	     pc._8bits.bit04,
	     pc._8bits.bit03,
	     pc._8bits.bit02,
	     pc._8bits.bit01,
	     pc._8bits.bit00,
	     pc._1byte,
	     pc._1byte,
	     flags.n,
	     flags.z
	     );
      break;

    case 2:
      if(args[2]) {
	sscanf(args[2], "%x", &mem_addr);
      }
	
      if(mem_addr < 0 || mem_addr >= MEM_SIZE)
	printf(error[1]);
      else
	for(i = mem_addr - 4;\
	    i <= mem_addr + 4 && (i >= -4 && i < MEM_SIZE);\
	    i++)
	  {
	    if(i < 0)
	      continue;

	    printf ("%02X ===> b: %d%d%d%d%d%d%d%d, d: %4d, h: %#10X",
		    i,
		    memory[i]._8bits.bit07,
		    memory[i]._8bits.bit06,
		    memory[i]._8bits.bit05,
		    memory[i]._8bits.bit04,
		    memory[i]._8bits.bit03,
		    memory[i]._8bits.bit02,
		    memory[i]._8bits.bit01,
		    memory[i]._8bits.bit00,
		    memory[i]._1byte,
		    memory[i]._1byte
		    );

	    if(i == mem_addr)
	      printf(" <===");
	    
	    printf("\n");
	  }

      break;

    default:
      printf(error[1]);
      break;
  }

  return 0;
}

int edit(const char *args[]) {
  int mem_addr, mem_cont, i, arg_code, found = 1;
  const char *edit_args[] = {
    "ac",
    "pc",
    "mem"
  };

  /* almost the same algorithm of print() */
  if(args[1])
    for(i = 0; i < 3; i++) {
      if(!strcmp(args[1], edit_args[i])) {
	arg_code = i;
	found = 0;
	break;
      }
    }

  if(found == 1)
    arg_code = NOT_FOUND;

  switch(arg_code) {
    case 0:
      if(args[2]) {
	sscanf(args[2], "%x", &mem_cont);
        ac._1byte = mem_cont;
      } else
	printf(error[1]);
      break;

    case 1:
      if(args[2]) {
	sscanf(args[2], "%x", &mem_cont);
        pc._1byte = mem_cont;
      } else
	printf(error[1]);
      break;

    case 2:
      if(args[2]) {
	sscanf(args[2], "%x", &mem_addr);

	if(mem_addr < 0 || mem_addr >= MEM_SIZE)
	  printf(error[1]);
	else
  	  if(args[3]) {
	    sscanf(args[3], "%x", &mem_cont);
	    memory[mem_addr]._1byte = mem_cont;
	    ac._1byte = mem_cont;
	  } else
	    printf(error[1]);
      } else
	printf(error[1]);
      break;

    default:
      printf(error[1]);
      break;
  }

  return 0;
}

int breakpoint(const char *args[]) {
  if(args[1])
    sscanf(args[1], "%x", &bp);
  else
    printf(error[1]);

  return 0;
}

int step(void) {
  step_instr();
  return 0;
}

/* Basic functionality of emulator, including HLT */
/* I'll fix this later... */
int run(void) {
  run_instr(bp);
  return 0;
}

int quit(void) {
  exit(0);
  return 0;  
}
