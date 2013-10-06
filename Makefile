
#
# PCLFilter, 2013 by Antonio Dell'elce, antonio@dellelce.com
#

CC             = gcc
TARGET         = lppagecnt

SHELL          = /bin/bash

CFILES         = src/main.c src/pcl.c src/pcl5_parser.c src/pcl_parse.c src/pclxl_parser.c src/pclxlfunctions.c src/pclxlvec.c
OFILES         = obj/main.o obj/pcl.o obj/pcl5_parser.o obj/pcl_parse.o obj/pclxl_parser.o obj/pclxlfunctions.o obj/pclxlvec.o
LDFLAGS	       = 


LOC_HFILES     = 
HFILES         = $(LOC_HFILES)

INCLUDES       =  -I. -I./includes
DEBUG          = 
CFLAGS         = -Wall -O2  $(INCLUDES) $(DEBUG)
LIBS           = 


#
# --- RULES ---
#

all: $(TARGET)

$(TARGET):   $(OFILES)
	@echo "LINK " $(TARGET)
	@$(CC) -o $(TARGET) $(LDFLAGS) $(OFILES) $(LIBS)

#
# -- DEPS --
#

obj/main.o: src/main.c $(HFILES) ./includes/pcl.h
	@echo "CC "src/main.c
	@$(CC) -c $(CFLAGS) -o obj/main.o src/main.c

obj/pcl.o: src/pcl.c $(HFILES) ./includes/pcl.h
	@echo "CC "src/pcl.c
	@$(CC) -c $(CFLAGS) -o obj/pcl.o src/pcl.c

obj/pcl5_parser.o: src/pcl5_parser.c $(HFILES) ./includes/pcl.h
	@echo "CC "src/pcl5_parser.c
	@$(CC) -c $(CFLAGS) -o obj/pcl5_parser.o src/pcl5_parser.c

obj/pcl_parse.o: src/pcl_parse.c $(HFILES) ./includes/pcl.h ./includes/pclxlfunc.h
	@echo "CC "src/pcl_parse.c
	@$(CC) -c $(CFLAGS) -o obj/pcl_parse.o src/pcl_parse.c

obj/pclxl_parser.o: src/pclxl_parser.c $(HFILES) ./includes/pcl.h ./includes/pclxlfunc.h
	@echo "CC "src/pclxl_parser.c
	@$(CC) -c $(CFLAGS) -o obj/pclxl_parser.o src/pclxl_parser.c

obj/pclxlfunctions.o: src/pclxlfunctions.c $(HFILES) ./includes/pcl.h
	@echo "CC "src/pclxlfunctions.c
	@$(CC) -c $(CFLAGS) -o obj/pclxlfunctions.o src/pclxlfunctions.c

obj/pclxlvec.o: src/pclxlvec.c $(HFILES) ./includes/pclxlfunc.h
	@echo "CC "src/pclxlvec.c
	@$(CC) -c $(CFLAGS) -o obj/pclxlvec.o src/pclxlvec.c

 
#  
# clean
#    
     
clean:
	rm -f $(TARGET) $(OFILES) $(LOC_HFILES) *.exe

#
# redo
#

redo: clean all

