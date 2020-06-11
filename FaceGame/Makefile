# Paths
OPENCV_PATH=/usr/local/include/opencv4/opencv

# Programs
CC=
CXX=g++ -std=c++11

# Flags
ARCH_FLAGS=

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)
CFLAGS=-Wextra -Wall $(ARCH_FLAGS) -O3 -Wno-long-long
LDFLAGS=$(ARCH_FLAGS)
DEFINES=
INCLUDES=-I$(OPENCV_PATH)/include -Iinclude/
LIBRARIES=-L$(OPENCV_PATH)/lib -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_highgui -lopencv_face
LINKER_FLAGS = -lGL -lGLU -lglut -lm -lSDL2 -lstdc++ -fpermissive -pthread
# Files which require compiling
SOURCE_FILES=\
	

# Source files which contain a int main(..) function
SOURCE_FILES_WITH_MAIN=src/FaceTracker.cpp

# End Configuration
SOURCE_OBJECTS=$(patsubst %.cpp,%.o,$(SOURCE_FILES))

ALL_OBJECTS=\
	$(SOURCE_OBJECTS) \
	$(patsubst %.cpp,%.o,$(SOURCE_FILES_WITH_MAIN))

DEPENDENCY_FILES=\
	$(patsubst %.o,%.d,$(ALL_OBJECTS)) 

all: build/bin/food_fighter

%.o: %.cpp Makefile
	@# Make dependecy file
	$(CXX) -MM -MT $@ -MF $(patsubst %.cpp,%.d,$<) $(CFLAGS) $(DEFINES) $(INCLUDES) $<
	@# Compile
	$(CXX) $(CFLAGS) $(DEFINES) -c -o $@ $< $(INCLUDES) $(LIBS)

-include $(DEPENDENCY_FILES)

build/bin/food_fighter: $(ALL_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(ALL_OBJECTS) audio.c $(LIBRARIES) $(LINKER_FLAGS) $(LIBS)

.PHONY: clean
clean:
	@echo "Cleaning"
	@for pattern in '*~' '*.o' '*.d' ; do \
		find . -name "$$pattern" | xargs rm ; \
	done
