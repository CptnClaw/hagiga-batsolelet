BIN=../../dev/gbdk-n-master/bin
OBJ=./obj
GAME=hagiga-batsolelet

build:
	mkdir -p $(OBJ)
	$(BIN)/gbdk-n-compile.sh $(GAME).c -o $(OBJ)/$(GAME).rel
	$(BIN)/gbdk-n-link.sh $(OBJ)/$(GAME).rel -o $(OBJ)/$(GAME).ihx
	$(BIN)/gbdk-n-make-rom.sh $(OBJ)/$(GAME).ihx $(GAME).gb

clean:
	rm -rf $(OBJ)
	rm -f $(GAME).gb
