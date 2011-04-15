SOURCES := $(shell ls *.cpp)

OBJS := $(SOURCES:.cpp=.o)
LIBS+=$(shell pkg-config --libs packagekit-glib2)
CFLAGS += -MD -DDBUS_API_SUBJECT_TO_CHANGE -DCALLBACK_DEBUG
CFLAGS += $(shell pkg-config --cflags packagekit-glib2)
CXXFLAGS:=$(CFLAGS)
all: package-kit

package-kit: $(OBJS)
	$(CC) $(CFLAGS) -o package-kit $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean: 
	rm -f $(OBJS) $(SOURCES:.cpp=.d) package-kit
