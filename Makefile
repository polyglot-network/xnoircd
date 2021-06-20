FLAGS = -pthread
OFLAGS = -g

xnoircd: user.o nameable.o channel.o connection.o ircserver.o irccommand.o userconnection.o main.o
	g++ $(FLAGS) user.o nameable.o channel.o connection.o ircserver.o irccommand.o userconnection.o main.o -o $@

user.o: user.cpp user.h
	g++ $(OFLAGS) -c $*.cpp

connection.o: connection.cpp connection.h
	g++ $(OFLAGS) -c $*.cpp

ircserver.o: ircserver.cpp ircserver.h
	g++ $(OFLAGS) -c $*.cpp

irccommand.o: irccommand.cpp irccommand.h
	g++ $(OFLAGS) -c $*.cpp

userconnection.o: userconnection.cpp userconnection.h
	g++ $(OFLAGS) -c $*.cpp

nameable.o: nameable.cpp nameable.h
	g++ $(OFLAGS) -c $*.cpp

channel.o: channel.cpp channel.h
	g++ $(OFLAGS) -c $*.cpp

main.o: main.cpp
	g++ $(OFLAGS) -c $*.cpp