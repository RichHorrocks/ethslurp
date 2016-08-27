top=../..

cflags=-Wall -O2 -DLINUX -I. -I$(top)/etherlib -I$(top)/daolib -I$(top)/utillib 
libs=$(top)/*/*.a /usr/lib/libcurl.dylib
dao=0xbb9bc244d798123fde783fcc1c72d3bb8c189413

exec=summation
product=objs/$(exec)
dest=.
src= \
summation.cpp \
acct_accums.cpp \
date_accums.cpp \
func_accums.cpp

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
#	@echo "building combine"
#	c++ -Wall -O2 -DLINUX -I../../utillib -c combine.cpp -o objs/combine.o
#	g++ -o combine objs/combine.o ../../*/*.a
#	@cd ../curator_watch ; make ; cd -

data:
	@./combine.1 >combine.fil
	@./combine >sumDAO_table.jade
	@mv *.jade ~/src.GitHub/quickblocks.io/views/
	@mv *.js ~/src.GitHub/quickblocks.io/public/js/

accts:
	cat ../../theData/theDAO/data/TheDAO.txt | cut -f3 | sort -u >accts_from.txt
	../../ethslurp -f:addrs $(dao) | tr '\t' '\n' | grep -v $(dao) | sort -u >accts_all.txt

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
