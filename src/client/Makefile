ARCH = amd64
TARGET = client
AS = i686-w64-mingw32-gcc
ASFLAGS = -c -g -DAMD64
ASRULE = $(AS) $(ASFLAGS) -o $@ $<
CC = i686-w64-mingw32-gcc
CFLAGS = -c -g -DAMD64
CCRULE = $(CC) $(CFLAGS) -o $@ $<
LD = i686-w64-mingw32-gcc
LDFLAGS = 
LDRULE = $(LD) $(LDFLAGS) -o $@ $^
DEP = i686-w64-mingw32-gcc
DEPRULE = $(DEP) -MM -o $@ $<;if [ $$? == 0 ];then echo $(OBJDIR)/$(ARCH)/`cat $@` > $@; fi
AFTER = 
OBJDIR = ../../obj/client
OUTPUT = ../../bin/$(ARCH)/client.exe
.PHONY : $(TARGET) all clean delete rebuild
all : $(TARGET)
sinclude $(OBJDIR)/$(ARCH)/main.dep
$(OBJDIR)/$(ARCH)/main.dep : main.c
	mkdir -p $(dir $@)
	$(DEPRULE)
$(OBJDIR)/$(ARCH)/main.o : main.c
	$(CCRULE)
$(OUTPUT) : $(OBJDIR)/$(ARCH)/main.o 
	mkdir -p $(dir $(OUTPUT))
	$(LDRULE)
$(TARGET) : $(OUTPUT)
	$(AFTER)
	rm -f  $(OBJDIR)/$(ARCH)/main.dep
clean :
	rm -rf $(OBJDIR)/$(ARCH)
delete :
	rm -rf $(OBJDIR)/$(ARCH)
	rm -rf $(OUTPUT)
rebuild :
	make delete
	make all
