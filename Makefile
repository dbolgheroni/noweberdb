# Makefile

COMPILER	= gcc -Wall -ledit -ltermcap
LIBS		= 
EXECUTABLE	= noweberdb
OBJECT		= debugger.o neander.o

$(EXECUTABLE): 	$(OBJECT)
		$(COMPILER) -o $(EXECUTABLE) $(OBJECT)

%.o:    	%.c
		$(COMPILER) -o $*.o -c $*.c

clean:
		rm -fr $(EXECUTABLE) *.o *.core *~