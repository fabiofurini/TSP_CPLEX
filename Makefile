# COMPILER

CXX= g++ 

# FILES

OBJS =	src/Main.o \
		src/MTZ_FORM.o \
		src/global_functions.o 
		
# CPLEX VERSION (LIBS and INCLUDE files)

#cplex 12.6 HERE
#CPLEXLIBDIR =  /home/fabio/ILOG/CPLEX_Studio_AcademicResearch126/cplex/lib/x86-64_linux/static_pic
#LP_INCLUDE= /home/fabio/ILOG/CPLEX_Studio_AcademicResearch126/cplex/include/ilcplex

CPLEXLIBDIR =  /Users/fabiofurini/Applications/IBM/ILOG/CPLEX_Studio126/cplex/lib/x86-64_osx/static_pic
LP_INCLUDE= /Users/fabiofurini/Applications/IBM/ILOG/CPLEX_Studio126/cplex/include/ilcplex


# Nothing should be changed

#LP_LIBS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread 
LP_LIBS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -lm -lpthread 
#LP_LIBS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -lm -lpthread -liberty -lbfd


DBG= -O3
#DBG= -g


INCDIR = -I. -I$(LP_INCLUDE)

#COMPILER FLAGS

CXXFLAGS =  $(DBG) $(INCDIR) 

.c.o:
	gcc -c $(CXXFLAGS) $< -o $@

LDLIBS = $(LP_LIBS)

all:TSP

TSP: $(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(OBJS): Makefile

clean:
	rm -f $(OBJS) rm TSP

