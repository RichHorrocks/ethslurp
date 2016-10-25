top=../..

cflags=-std=c++11 -Wall -O2 -DLINUX -I. -I$(top)/etherlib -I$(top)/daolib -I$(top)/utillib 
libs=$(top)/*/*.a /usr/lib/libcurl.dylib
dao=0xbb9bc244d798123fde783fcc1c72d3bb8c189413
#dao=0x7cB57B5A97eAbe94205C07890BE4c1aD31E486A8

exec=summation
product=objs/$(exec)
dest=.
src= \
accumulator.cpp \
charts.cpp \
funcMap.cpp \
summation.cpp \
tables.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

all:
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@mv $(product) $(dest)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

data:
	@make
#	@make accts
#	@cd ../curator_watch ; make ; cd -
#	ethprice -c -p:30 -f -v
	@./summation
	@mv *.js ~/src.GitHub/quickblocks.io/subdomains/dao/public/js/
	@mv *.jade ~/src.GitHub/quickblocks.io/subdomains/dao/views/

accts:
	../../ethslurp -f:addrs $(dao) | tr '\t' '\n' | grep -v $(dao) | sort -u >accts_all.txt

$(product): $(objects) $(libs)
	g++ -o $(product) $(objects) $(libs)
	@strip $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o: %.cpp
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
	@rm -f $(product) objs/*
	@rm -f ~/src.GitHub/quickblocks.io/subdomains/dao/public/js/detail*Chart*.js
	@rm -f ~/src.GitHub/quickblocks.io/subdomains/dao/public/js/overviewChart*.js
#	@rm -f ~/src.GitHub/quickblocks.io/subdomains/dao/views/detail*Table*.jade
	@rm -f ~/src.GitHub/quickblocks.io/subdomains/dao/views/overviewTable*.jade
