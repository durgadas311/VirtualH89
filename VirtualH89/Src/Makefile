#SOURCES = $(filter-out mms77320.cpp, $(filter-out GenericSASIDrive.cpp,$(wildcard *.cpp)))
SOURCES = $(wildcard *.cpp)

OBJECTS = $(subst .cpp,.o,$(SOURCES))
EXEOBJS = $(filter-out JNIProxyConsole.o,$(OBJECTS))
V89NAME = v89

all: $(V89NAME)

$(V89NAME): $(EXEOBJS)
	$(CXX) -o $@ $(EXEOBJS) -lpthread -lGL -lglut

CXXFLAGS = -MMD -g -O3 -std=c++11

clean:
	rm -f *.o *.lo

clobber: clean
	rm -f *.d $(V89NAME)

uncrust:
	uncrustify -q --no-backup -c ../uncrust.cfg *.cpp *.h

-include *.d

JNIOBJS = $(subst .o,.lo,$(OBJECTS))
JNIOBJS := $(filter-out main.o,$(JNIOBJS))
JNIOBJS := $(filter-out StdioConsole.o,$(JNIOBJS))
JNIOBJS := $(filter-out StdioProxyConsole.o,$(JNIOBJS))
JNIOBJS := $(filter-out h19.o,$(JNIOBJS))

jni: libV89JNI.so

libV89JNI.so: $(JNIOBJS)
	$(CXX) -fPIC -shared -o libV89JNI.so $(JNIOBJS)

%.lo: %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -c -o $@ $<
