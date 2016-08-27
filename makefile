cflags=-Wall -O2 -DLINUX -I. -I./utillib -I./etherlib -I./daolib
libs=*/*.a /usr/lib/libcurl.dylib

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
	@cd etherlib; make; cd ..
	@cd daolib; make; cd ..
	@cd src/ethname; make; cd ../..
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

test: all
	@cd src/ethname; make test ; cd -
	@cd theData/tests; make; cd -
	@cd theData/theDAO; make; cd -
	@cd theData/funcs; make; cd -
	@cd theData/ghc; make; cd -
	@cd theData/otherContracts; make; cd -
	@cd theData/whales; make; cd -
	@cd theData/hack; make; cd -
	@make -B back

back:
	@rm -fR theData/backup
	@cp -pR ~/.ethslurp theData/backup
	@rm -fR theData/backup/slurps

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
	@cd utillib; make clean; cd ..
	@cd etherlib; make clean; cd ..
	@cd daolib; make clean; cd ..
	@cd src/ethname; make clean; cd ../..
	-@$(RM) -f $(product) $(objs)/*.o $(objs)/*.a 2> /dev/null
