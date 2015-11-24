JSONRPCLIB=libjsonrpc.a
CFLAGS=-I../include/
LIBSRC=jsonrpc.cc json_reader.cc jsonrpc_handler.cc json_value.cc  json_writer.cc
LIBOBJ=$(addprefix ./, $(addsuffix .o, $(basename $(LIBSRC))))
all: $(JSONRPCLIB)
%.o: %.cc
	g++ $(CFLAGS) -o $@ -c $^
$(JSONRPCLIB):$(LIBOBJ)
	ar -rcs $@ $^

clean:
	rm $(LIBOBJ) $(JSONRPCLIB) -f
