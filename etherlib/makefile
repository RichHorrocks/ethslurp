cflags=-std=c++11 -Wall -O2 -DLINUX -I../utillib -I../abilib -I../nodelib -I./

product=etherlib.a

src= \
account.cpp \
blockchain.cpp \
block.cpp \
logentry.cpp \
pricequote.cpp \
receipt.cpp \
transaction.cpp \
trace.cpp \
structlog.cpp \
webapi.cpp \
etherlib.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

all:
	@echo "$(product) build started"
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@make $(product)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

$(product): $(objects)
	-@$(RM) -f $*.a
	ar qc $*.a $(objects)
#	@chown jrush.jrush $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

clean:
	-@$(RM) -f $(dest) $(product) objs/* 2> /dev/null
