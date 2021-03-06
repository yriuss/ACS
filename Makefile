include /usr/local/etc/PcapPlusPlus.mk

LIBS_ARC=-lpigpio -lrt -lpthread
LIBS_PC=-lrt -lpthread
LIBS=$(LIBS_PC)

all:
#Communication
	g++ -c TTcpIPServer.cpp -pthread -I. -o TServer.o
	g++ -c TTcpIPClient.cpp -pthread -I. -o TClient.o
#Attenuation and Ph shifts settings
	g++ -c ToolBoxAndUtilities.cpp -o ToolBoxAndUtilities.o
	g++ -c Serial.cpp -o Serial.o
	g++ -c Attenuator.cpp -o Attenuator.o 
	g++ -c TTable.cpp -o TTable.o
	g++ -c TPhShifter.cpp -o TPhShifter.o
	g++ -c TCommand.cpp -o TCommand.o
	g++ $(PCAPPP_INCLUDES) -c TAcs.cpp -o TAcs.o
	g++ $(PCAPPP_INCLUDES) -c -o main.o main.cpp

	g++ -pthread $(PCAPPP_LIBS_DIR) -static-libstdc++ -o acs -I. main.o TServer.o TClient.o TCommand.o TPhShifter.o TAcs.o TTable.o ToolBoxAndUtilities.o Serial.o Attenuator.o $(PCAPPP_LIBS) -lrt -lpthread

run: 
	sudo ./acs
	
clean:
	rm -f ToolBoxAndUtilities.o
	rm -f Attenuator.o
	rm -f Serial.o
	rm -f TTable.o
	rm -f TPhShifter.o
	rm -f TAcs.o
	rm -f TCommand.o
	rm -f main.o
	rm acs