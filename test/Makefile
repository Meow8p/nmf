CC=gcc
CFLAGS=-Wall -O0 -finstrument-functions -DNMF_DETAILED_DEBUG
OUTDIR=.

all:
	$(CC) $(CFLAGS) main.c ../src/libnmf.c -o $(OUTDIR)/testNMF

clean:
	rm -vr $(OUTDIR)/testNMF
