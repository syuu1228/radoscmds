PROGRAMS = create_bucket delete_bucket list_bucket put_obj get_obj delete_obj list_obj
CXXFLAGS = -Wall -Werror -D_FILE_OFFSET_BITS=64 -g
LDFLAGS = -lcrypto -lrados -lcrush

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

create_bucket: create_bucket.o

delete_bucket: delete_bucket.o

list_bucket: list_bucket.o

put_obj: put_obj.o

get_obj: get_obj.o

delete_obj: delete_obj.o 

list_obj: list_obj.o
