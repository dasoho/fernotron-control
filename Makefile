all: FernotronSniffer FernotronSend

FernotronSniffer: Fernotron.o FernotronSniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
	
FernotronSend: Fernotron.o FernotronSend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi	

clean:
	$(RM) *.o FernotronSniffer FernotronSend