OBJDIR := obj
SRCDIR := src
objects := $(OBJDIR)/main.o $(OBJDIR)/work.o $(OBJDIR)/DES.o $(OBJDIR)/assist.o 

myDES: $(objects)
	gcc -o myDES $(objects) 

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/global.h $(SRCDIR)/assist.h $(SRCDIR)/work.h
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi;
	gcc -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/assist.o: $(SRCDIR)/assist.c
	gcc -c $(SRCDIR)/assist.c -o $(OBJDIR)/assist.o

$(OBJDIR)/DES.o: $(SRCDIR)/DES.c $(SRCDIR)/global.h $(SRCDIR)/assist.h $(SRCDIR)/matrix.h
	gcc -c $(SRCDIR)/DES.c -o $(OBJDIR)/DES.o

$(OBJDIR)/work.o: $(SRCDIR)/work.c $(SRCDIR)/global.h $(SRCDIR)/assist.h $(SRCDIR)/DES.h
	gcc -c $(SRCDIR)/work.c -o $(OBJDIR)/work.o



.PHONY: clean

clean:
	rm -rf myDES $(OBJDIR)