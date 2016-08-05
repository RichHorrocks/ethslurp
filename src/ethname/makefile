cflags=-Wall -O2 -DLINUX -I. -I../../etherlib -I../../daolib -I../../utillib 
libs=../../*/*.a /usr/lib/libcurl.dylib

exec=ethname
product=objs/$(exec)
dest=~/source/$(exec)

src= \
ethname.cpp \
name_options.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

all:
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@mv $(product) $(dest)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

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
	@cd ../../utillib ;  make ; cd -
	@cd ../../daolib ; make ; cd -
	@cd ../../etherlib ; make ; cd -

cleanall:
	@cd ../../utillib ; make clean ; cd -
	@cd ../../daolib ; make clean ; cd -
	@cd ../../etherlib ; make clean ; cd -
	@make clean

clean:
	-@$(RM) -f $(dest) $(product) objs/* 2> /dev/null
