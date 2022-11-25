CC     := gcc
CFLAGS := -Wall -Wextra -g
RPCLIB := -ltirpc -I/usr/include/tirpc


all: reader-writer rpc

clean:
	rm -rf bin/

.PHONY: all clean


reader-writer: mkdir
	$(CC) $(CFLAGS) src/reader-writer/reader-writer.c -o bin/reader-writer

rpc: mkdir
	cd src/rpc && \
	rpcgen math.x && \
	$(CC) $(CFLAGS) $(RPCLIB) math_svc.c math_xdr.c server.c -o ../../bin/math-server && \
	$(CC) $(CFLAGS) $(RPCLIB) math_clnt.c math_xdr.c client.c -o ../../bin/math-client

mkdir:
	mkdir -p bin/
