#SOURCES = $(filter-out mms77320.cpp, $(filter-out GenericSASIDrive.cpp,$(wildcard *.cpp)))
SOURCES = $(wildcard *.cpp)

OBJECTS = $(subst .cpp,.o,$(SOURCES))
V89NAME = v89

all: $(V89NAME)

$(V89NAME): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) -lpthread -lGL -lglut

CXXFLAGS = -MMD -g -O3 -std=c++11

clean:
	rm -f *.o

clobber: clean
	rm -f *.d $(V89NAME)

uncrust:
	uncrustify -q --no-backup -c ../uncrust.cfg *.cpp *.h

-include *.d

jni: V89JNI.class libV89JNI.so

V89JNI.class: V89JNI.java
	javac V89JNI.java

V89JNI.h: V89JNI.class
	javah -classpath . V89JNI
	@touch $@

libV89JNI.so: V89JNI.cpp V89JNI.h  
	g++ -fPIC -shared -o libV89JNI.so  V89JNI.cpp
