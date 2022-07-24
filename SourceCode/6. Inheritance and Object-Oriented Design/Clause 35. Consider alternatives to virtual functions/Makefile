CC := g++


BINFOLDER := bin
EXEFOLDER := $(BINFOLDER)\exe
OBJFOLDER := $(BINFOLDER)\obj

TARGET := $(EXEFOLDER)\Application.exe
APPOBJ := $(OBJFOLDER)\Application.obj
VIRFUNCOBJ := $(OBJFOLDER)\VirFunc.obj
NVIOBJ := $(OBJFOLDER)\NVI.obj
FUNCPTROBJ := $(OBJFOLDER)\FuncPtr.obj
FUNCOBJ := $(OBJFOLDER)\FuncObj.obj
STROBJ := $(OBJFOLDER)\Strategy.obj



$(TARGET): $(APPOBJ) $(VIRFUNCOBJ) $(NVIOBJ) $(FUNCPTROBJ) $(FUNCOBJ) $(STROBJ)
	@$(CC) $^ -o  $@


$(APPOBJ): Application.cpp
	@$(CC) -c $^ -o $@

$(VIRFUNCOBJ): VirFunc\VirFunc.cpp
	@$(CC) -c $^ -o $@

$(NVIOBJ): NVI\NVI.cpp
	@$(CC) -c $^ -o $@

$(FUNCPTROBJ): FuncPtr\FuncPtr.cpp
	@$(CC) -c $^ -o $@

$(FUNCOBJ): FuncObj\FuncObj.cpp
	@$(CC) -c $^ -o $@

$(STROBJ): Strategy\Strategy.cpp
	@$(CC) -c $^ -o $@



.PHYON: cleanObj cleanExe clean run

cleanObj:
	del $(OBJFOLDER)\*.obj

cleanExe:
	del $(EXEFOLDER)\*.exe

clean:
	del $(OBJFOLDER)\*.obj
	del $(EXEFOLDER)\*.exe

run:
	@./$(TARGET)
