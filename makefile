CC=g++
CFLAGS=-I
BDIR = /build
IDIR = /include
SDIR = /src
SROOT = /Server
CROOT = /Client
CBUILD = .$(CROOT)$(BDIR)
SBUILD = .$(SROOT)$(BDIR)
CSRC = .$(CROOT)$(SDIR)
SSRC = .$(SROOT)$(SDIR)
_DEPS_S = server_utils.h
DEPS = $(patsubst %,$(SROOT)$(IDIR)/%,$(_DEPS_S)) 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: basic_server

basic_server: $(SSRC)/basic_server.o $(SSRC)/server_utils.o
	$(CC) -o $(SBUILD)/basic_server $(SSRC)/basic_server.o $(SSRC)/server_utils.o

sample_server: 