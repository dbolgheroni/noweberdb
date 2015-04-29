noweberdb is a free, BSD-licensed simulator for the computers Neander, Ahmes, Ramses and Cesar found in the book "Weber, Raul Fernando. Fundamentos de arquitetura de computadores. Editora Sagra Lazzato, Porto Alegre, 2000.".

It should be portable to POSIX-complaint environments. In other words, should compile without modifications.

This very simple computer with 11 instructions do not even have an I/O interface with external world (that's why it has "db" at the end of the name, since the only way you can interact with computer is with the debugger). This is not a limitation of the simulator itself. Neander is, indeed, a very simple, didactic computer to start.

noweberdb should evolve to simulate more complex computers published in the same book like Ahmes, Ramses and Cesar. Cesar is almost identical to a PDP-11 architecture. It has a display. Nice.