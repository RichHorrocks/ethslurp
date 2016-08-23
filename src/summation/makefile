top=../..

cflags=-Wall -O2 -DLINUX -I. -I$(top)/etherlib -I$(top)/daolib -I$(top)/utillib 
libs=$(top)/*/*.a /usr/lib/libcurl.dylib

exec=summation
product=objs/$(exec)
dest=.
src=summation.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

all:
	touch *.cpp
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@mv $(product) $(dest)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

data:
	@mv *.js ~/src.GitHub/quickblocks.io/public/js/

$(product): $(objects) $(libs)
	g++ -o $(product) $(objects) $(libs)
	@strip $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

test:
	@./do_tests

libs:
	@cd $(top)/utillib ;  make ; cd -
	@cd $(top)/daolib ; make ; cd -
	@cd $(top)/etherlib ; make ; cd -

cleanall:
	@cd $(top)/utillib ; make clean ; cd -
	@cd $(top)/daolib ; make clean ; cd -
	@cd $(top)/etherlib ; make clean ; cd -
	@make clean

clean:
	@rm -f $(dest) $(product) objs/* 2> /dev/null
