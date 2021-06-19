FLAGS = -pthread

xnoircd: user.o nameable.o channel.o connection.o ircserver.o irccommand.o userconnection.o main.o
	g++ $(FLAGS) user.o nameable.o channel.o connection.o ircserver.o irccommand.o userconnection.o main.o -o $@

user.o: user.cpp user.h
	g++ -g -c $*.cpp

connection.o: connection.cpp connection.h
	g++ -g -c $*.cpp

ircserver.o: ircserver.cpp ircserver.h
	g++ -g -c $*.cpp

irccommand.o: irccommand.cpp irccommand.h
	g++ -g -c $*.cpp

userconnection.o: userconnection.cpp userconnection.h
	g++ -g -c $*.cpp

nameable.o: nameable.cpp nameable.h
	g++ -g -c $*.cpp

channel.o: channel.cpp channel.h
	g++ -g -c $*.cpp

main.o: main.cpp
	g++ -g -c $*.cpp