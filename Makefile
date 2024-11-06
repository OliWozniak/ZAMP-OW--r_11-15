__start__: obj __lines_for_space__ interp __plugin__
	export LD_LIBRARY_PATH="./libs"; ./interp program_dzialan config/config.xml

obj:
	mkdir -p obj  

__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo

__plugin__:
	cd plugin; make

CPPFLAGS = -Wall -g -pedantic -std=c++17 -Iinc
LDFLAGS = -Wall -lxerces-c -ldl  

interp: obj/main.o obj/LibInterface.o obj/xmlinterp.o obj/Set4LibInterfaces.o
	g++ -o interp obj/main.o obj/LibInterface.o obj/xmlinterp.o obj/Set4LibInterfaces.o $(LDFLAGS)

obj/main.o: src/main.cpp inc/AbstractInterp4Command.hh inc/AbstractScene.hh \
            inc/AbstractComChannel.hh inc/LibInterface.hh inc/xmlinterp.hh \
			inc/Set4LibInterfaces.hh 
	mkdir -p obj  
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

obj/LibInterface.o: src/LibInterface.cpp inc/LibInterface.hh  
	mkdir -p obj  
	g++ -c ${CPPFLAGS} -o obj/LibInterface.o src/LibInterface.cpp

obj/Set4LibInterfaces.o: src/Set4LibInterfaces.cpp inc/Set4LibInterfaces.hh  
	mkdir -p obj  
	g++ -c ${CPPFLAGS} -o obj/Set4LibInterfaces.o src/Set4LibInterfaces.cpp

obj/xmlinterp.o: src/xmlinterp.cpp inc/xmlinterp.hh 
	mkdir -p obj  
	g++ -c ${CPPFLAGS} -o obj/xmlinterp.o src/xmlinterp.cpp


doc:
	cd dox; make


clean:
	rm -f obj/* interp core*

clean_plugin:
	cd plugin; make clean

cleanall: clean
	cd plugin; make cleanall
	cd dox; make cleanall
	rm -f libs/*
	find . -name \*~ -print -exec rm {} \;


help:
	@echo
	@echo "  Lista podcelów dla polecenia make"
	@echo 
	@echo "        - (wywołanie bez specyfikacji celu) wymusza"
	@echo "          kompilację i uruchomienie programu."
	@echo "  clean    - usuwa produkty kompilacji oraz program"
	@echo "  clean_plugin - usuwa plugin"
	@echo "  cleanall - wykonuje wszystkie operacje dla podcelu clean oraz clean_plugin"
	@echo "             oprócz tego usuwa wszystkie kopie (pliki, których nazwa "
	@echo "             kończy się znakiem ~)."
	@echo "  help  - wyświetla niniejszy komunikat"
	@echo
	@echo " Przykładowe wywołania dla poszczególnych wariantów. "
	@echo "  make           # kompilacja i uruchomienie programu."
	@echo "  make clean     # usuwa produkty kompilacji."
	@echo
