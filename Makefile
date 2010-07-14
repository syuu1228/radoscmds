PROGRAMS = create_bucket delete_bucket list_bucket
CXXFLAGS = -lcrypto -lrados -lcrush

all: $(PROGRAMS)
clean:
	rm -f $(PROGRAMS)
