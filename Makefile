OBJDIR := obj
SRCDIR := src
objects := $(OBJDIR)/main.o $(OBJDIR)/global.o  $(OBJDIR)/assist.o \
           $(OBJDIR)/DES.o $(OBJDIR)/Bignum.o $(OBJDIR)/RSA.o $(OBJDIR)/work.o 

myDES: $(objects)
	g++ -o myDES $(objects) 

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp \
       $(SRCDIR)/global.h $(SRCDIR)/assist.h $(SRCDIR)/RSA.h $(SRCDIR)/work.h
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi;
	g++ -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/work.o: $(SRCDIR)/work.cpp \
                  $(SRCDIR)/global.h $(SRCDIR)/assist.h $(SRCDIR)/DES.h $(SRCDIR)/RSA.h
	g++ -c $(SRCDIR)/work.cpp -o $(OBJDIR)/work.o

$(OBJDIR)/DES.o: $(SRCDIR)/DES.cpp \
                 $(SRCDIR)/global.h $(SRCDIR)/matrix.h $(SRCDIR)/assist.h
	g++ -c $(SRCDIR)/DES.cpp -o $(OBJDIR)/DES.o

$(OBJDIR)/RSA.o: $(SRCDIR)/RSA.cpp \
                 $(SRCDIR)/global.h $(SRCDIR)/Bignum.h
	g++ -c $(SRCDIR)/RSA.cpp -o $(OBJDIR)/RSA.o

$(OBJDIR)/Bignum.o: $(SRCDIR)/Bignum.cpp $(SRCDIR)/Bignum.h
	g++ -c $(SRCDIR)/Bignum.cpp -o $(OBJDIR)/Bignum.o

$(OBJDIR)/assist.o: $(SRCDIR)/assist.cpp
	g++ -c $(SRCDIR)/assist.cpp -o $(OBJDIR)/assist.o

$(OBJDIR)/global.o: $(SRCDIR)/global.cpp
	g++ -c $(SRCDIR)/global.cpp -o $(OBJDIR)/global.o

.PHONY: clean

clean:
	rm -rf myDES $(OBJDIR)