CC=gcc
CFLAGS=
INCLUDEPATH=../src/include
LIBPATH=-L../build
INCLUDE=-I$(INCLUDEPATH)
LIBS=-lstdc++ -lm -larboretum
SRC= mccatch.cpp app.cpp graph.cpp Application.cpp
OBJS=$(subst .cpp,.o,$(SRC))

STD=-std=c++20

# Implicit Rules
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(STD) -c $< -o $@ $(INCLUDE)

McCatch: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o mccatch $(INCLUDE) $(LIBPATH) $(LIBS)

clean:
	rm -f *.o
	rm -f mccatch
	rm -f SlimTree*.dat