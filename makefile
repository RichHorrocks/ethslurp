ethslurp=.
cflags=-std=c++11 -Wall -O2 -DLINUX -I. -I$(ethslurp)/utillib -I$(ethslurp)/abilib -I$(ethslurp)/etherlib

# for mac builds
libs=*/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=*/*.a -lcurl

product=ethslurp
src= \
ethslurp.cpp \
slurp_options.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))
objs=./objs

all:
	@cd utillib; make; cd ..
	@cd abilib; make; cd ..
	@cd etherlib; make; cd ..
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

data: test

test: all
	@cd theData/tests; make; cd -
	@cd theData/theDAO; make; cd -
	@cd theData/funcs; make; cd -
	@cd theData/ghc; make; cd -
	@cd theData/otherContracts; make; cd -
	@cd theData/whales; make; cd -
	@cd theData/hack; make; cd -
	@make -B back

#	@cd src/summation ; make data ; cd -

back:
	@rm -fR theData/backup
	@mkdir theData/backup
	@cp -pR ~/.ethslurp/abis          theData/backup/
	@cp -pR ~/.ethslurp/configs       theData/backup/
	@cp -pR ~/.ethslurp/slurps        theData/backup/
	@cp -p  ~/.ethslurp/ethslurp.conf theData/backup/
	@cp -p  ~/.ethslurp/prices.bin    theData/backup/

$(product): $(objects) $(libs)
	$(CXX) -o $(product) $(objects) $(libs)
	@strip $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

cleanall:
	@make clean

clean:
	@cd utillib; make clean; cd ..
	@cd abilib; make clean; cd ..
	@cd etherlib; make clean; cd ..
	-@$(RM) -f $(product) $(objs)/*.o $(objs)/*.a 2> /dev/null
