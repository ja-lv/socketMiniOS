BDIR = /build
IDIR = /include
SDIR = /src
_CONFIG = ./_Config/
SROOT = /Server
CROOT = /Client
CBUILD = .$(CROOT)$(BDIR)
SBUILD = .$(SROOT)$(BDIR)
CSRC = .$(CROOT)$(SDIR)
SSRC = .$(SROOT)$(SDIR)
_DEPS_S = server_utils.h 
DEPS = $(patsubst %,.$(SROOT)$(IDIR)/%,$(_DEPS_S))

CC=g++
CFLAGS=-I

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS)$(_CONFIG) -c -o $@ $<

all: basic_server disk_server

basic_server: $(SSRC)/basic_server.o $(SSRC)/server_utils.o
	$(CC) $(CFLAGS)$(_CONFIG) -o $(SBUILD)/basic_server $(SSRC)/basic_server.o $(SSRC)/server_utils.o

disk_server: $(SSRC)/disk_server.o $(SSRC)/server_utils.o
	$(CC) $(CFLAGS)$(_CONFIG) -o $(SBUILD)/disk_server $(SSRC)/disk_server.o $(SSRC)/server_utils.o