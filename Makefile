include rule.mk

CFLAGS	= -Wall -std=c99

INCDIR = ./
LIBDIR = ./
OBJDIR = ./obj/
LIBOBJ = $(addprefix $(OBJDIR), $(addsuffix .o, $(basename $(wildcard *.c))))
SLIB	= $(OBJDIR)libvbus.a

PHONY += all install help

#all: slib
#slib: $(SLIB)
all: $(SLIB)

$(OBJDIR)%.o: %.c
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -c $< -o $@ 
	
$(SLIB): $(LIBOBJ)
	$(AR) rcu $@ $^
	$(RANLIB) $@
	@test -d $(LIBDIR) || mkdir -p $(LIBDIR)
	cp $@ $(LIBDIR)

install:
	#@test -d $(LIBDIR) || mkdir -p $(LIBDIR)
	#cp $(SLIB) $(LIBDIR)
	
clean:
	rm -rf $(SLIB) $(LIBOBJ) $(OBJDIR)

help:
	@echo "|-----------------------usage-----------------------|"
	@echo " make: make the vbus lib "
	@echo " make clean: clean all complie files"
	@echo " make install [INSTALL_DIR=?]: install application "
	@echo " [INSTALL_DIR] app is installed in '/usr/dbin/' by default"
	@echo "|---------------------------------------------------|"