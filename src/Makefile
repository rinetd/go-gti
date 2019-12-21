
export GOARCH=arm64
export GOOS=linux
SRCS= *.cpp
OBJS= *.o
LIBS= libgti.a

.PHONY: all clean
all: install

libgti.a:
	g++ -c -Wall -g -fPIC -L$(CURDIR)/lib/linux/aarch64 -IInclude  ${SRCS}
	ar -rvs libgti.a ${OBJS}

shared:
	# g++ -shared -fPIC  ./lib/linux/aarch64/libGTILibrary.so libftd3xx.so -pthread -o libgti.so  ${SRCS}
	g++ -shared -fPIC -L$(CURDIR)/lib/linux/aarch64 -lGTILibrary -pthread -o libgti.so  ${SRCS}

install: clean shared
	# cp $(CURDIR)/gti.h ../
	# 生成cgo使用的动态库文件
	cp $(CURDIR)/*.so ../lib 
	cp $(CURDIR)/lib/linux/aarch64/*.so ../lib 
	cp $(CURDIR)/lib/linux/aarch64/*.so.* ../lib 

clean:
	rm -f $(CURDIR)/../lib/*
	-rm -f ${OBJS} libgti.a $(CURDIR)/../lib/libgti.a $(CURDIR)/../lib/libgti.so