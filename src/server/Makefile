ARCH = amd64
TARGET = server
AS = gcc
ASFLAGS = -c -g -DAMD64
ASRULE = $(AS) $(ASFLAGS) -o $@ $<
CC = gcc
CFLAGS = -c -g -DAMD64
CCRULE = $(CC) $(CFLAGS) -o $@ $<
LD = gcc
LDFLAGS = 
LDRULE = $(LD) $(LDFLAGS) -o $@ $^
DEP = gcc
DEPRULE = $(DEP) -MM -o $@ $<;if [ $$? == 0 ];then cat $@|awk 'NR == 1 {printf("%s%s\n","$(OBJDIR)/$(ARCH)/",$$0)} NR != 1 {print $$0}' > $@; fi
AFTER = 
OBJDIR = ../../obj/server
OUTPUT = ../../bin/$(ARCH)/server
.PHONY : $(TARGET) all clean delete rebuild
all : $(TARGET)
sinclude $(OBJDIR)/$(ARCH)/main.dep
$(OBJDIR)/$(ARCH)/main.dep : main.c
	mkdir -p $(dir $@)
	$(DEPRULE)
$(OBJDIR)/$(ARCH)/main.o : main.c
	$(CCRULE)
sinclude $(OBJDIR)/$(ARCH)/common/common/list/list.dep
$(OBJDIR)/$(ARCH)/common/common/list/list.dep : common/common/list/list.c
	mkdir -p $(dir $@)
	$(DEPRULE)
$(OBJDIR)/$(ARCH)/common/common/list/list.o : common/common/list/list.c
	$(CCRULE)
sinclude $(OBJDIR)/$(ARCH)/common/common/map/map.dep
$(OBJDIR)/$(ARCH)/common/common/map/map.dep : common/common/map/map.c
	mkdir -p $(dir $@)
	$(DEPRULE)
$(OBJDIR)/$(ARCH)/common/common/map/map.o : common/common/map/map.c
	$(CCRULE)
$(OUTPUT) : $(OBJDIR)/$(ARCH)/main.o $(OBJDIR)/$(ARCH)/common/common/list/list.o $(OBJDIR)/$(ARCH)/common/common/map/map.o 
	mkdir -p $(dir $(OUTPUT))
	$(LDRULE)
$(TARGET) : $(OUTPUT)
	$(AFTER)
	rm -f  $(OBJDIR)/$(ARCH)/main.dep $(OBJDIR)/$(ARCH)/common/common/list/list.dep $(OBJDIR)/$(ARCH)/common/common/map/map.dep
clean :
	rm -rf $(OBJDIR)/$(ARCH)
delete :
	rm -rf $(OBJDIR)/$(ARCH)
	rm -rf $(OUTPUT)
rebuild :
	make delete
	make all
