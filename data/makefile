top=../

cflags=-std=c++11 -Wall -O2 -DLINUX -I. -I$(top)/utillib -I$(top)/etherlib -I$(top)/daolib
libs=$(top)/*/*.a /usr/lib/libcurl.dylib

product=shit
src=shit.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

all:
	@cd $(top)/utillib; make; cd -
	@cd $(top)/etherlib; make; cd -
	@cd $(top)/daolib; make; cd -
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@./$(product)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

$(product): $(objects) $(libs)
	g++ -o $(product) $(objects) $(libs)
	@strip $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

cleanall:
	@make clean

clean:
	@cd $(top)/utillib; make clean; cd -
	@cd $(top)/etherlib; make clean; cd -
	@cd $(top)/daolib; make clean; cd -
	-@$(RM) -f $(dest) $(product) objs/* 2> /dev/null
