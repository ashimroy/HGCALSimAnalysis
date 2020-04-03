objects = main.o 
HGCTB18_SIM.exe : $(objects)
	g++ -o HGCTB18_SIM.exe $(objects) `root-config --libs`
main.o : main.C HGCTB18_SIM.h HGCTB18_SIM.C HGCalTBTopology.C
	g++ -Wall -Wno-deprecated `root-config --cflags` main.C -c
clean :
	rm HGCTB18_SIM.exe $(objects)
