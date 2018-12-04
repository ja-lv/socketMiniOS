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

all: basic_server

basic_server: $(SSRC)/basic_server.o $(SSRC)/server_utils.o
	$(CC) -o $(SBUILD)/basic_server $(SSRC)/basic_server.o $(SSRC)/server_utils.o