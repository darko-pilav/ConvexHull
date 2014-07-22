# Environment
MKDIR=mkdir
RM=rm
CXX=g++

# Flags
DEBUG = -g
CXXFLAGS = -std=c++0x -c -Wall $(DEBUG)
LFLAGS = -lm -lGL -lGLU -lglut -Wall $(DEBUG)

# Object Directory
OBJECTDIR = ./obj
EXECUTABLE = convexhull

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/HeapSort.o \
	${OBJECTDIR}/Point.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Hull2D.o

# Build Targets
${EXECUTABLE}: ${OBJECTFILES}
	${CXX} ${OBJECTFILES} -o ${EXECUTABLE} ${LFLAGS}

${OBJECTDIR}/HeapSort.o: HeapSort.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(CXX) ${CXXFLAGS} -o ${OBJECTDIR}/HeapSort.o HeapSort.cpp

${OBJECTDIR}/Point.o: Point.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(CXX) ${CXXFLAGS} -o ${OBJECTDIR}/Point.o Point.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(CXX) ${CXXFLAGS} -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Hull2D.o: Hull2D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(CXX) ${CXXFLAGS} -o ${OBJECTDIR}/Hull2D.o Hull2D.cpp

clean: 	
	${RM} ${OBJECTDIR}/*.o ${EXECUTABLE}
