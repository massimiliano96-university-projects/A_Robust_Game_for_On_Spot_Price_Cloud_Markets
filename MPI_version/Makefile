CXX = mpicxx

CXXFLAGS += -Wall -std=c++11 -I/opt/gurobi901/linux64/include -L/opt/gurobi901/linux64/lib -lgurobi_c++ -lgurobi90


EXE = exe

OBJS = main.o Game.o Set_System.o SaaS_Problem.o SaaS.o application.o WS.o IaaS.o IaaS_Problem.o

.DEFAULT_GOAL = all

main.o Game.o Set_System.o SaaS_Problem.o SaaS.o application.o WS.o : WS.hpp
main.o Game.o Set_System.o SaaS_Problem.o SaaS.o application.o : application.hpp
main.o Game.o Set_System.o SaaS_Problem.o SaaS.o : SaaS.hpp
main.o Game.o Set_System.o SaaS_Problem.o : SaaS_Problem.hpp
main.o Game.o Set_System.o IaaS_Problem.o IaaS.o: IaaS.hpp
main.o Game.o Set_System.o IaaS_Problem.o: IaaS_Problem.hpp
main.o Game.o Set_System.o : Set_System.hpp
main.o Game.o : Game.hpp

all : build

.PHONY : all build clean distclean

build : $(EXE)

$(EXE) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ $(OUTPUT_OPTION) -I/opt/gurobi901/linux64/include -L/opt/gurobi901/linux64/lib -lgurobi_c++ -lgurobi90

clean:
	$(RM) *.o

distclean: clean
	$(RM)$(EXE)
	$(RM)*~
