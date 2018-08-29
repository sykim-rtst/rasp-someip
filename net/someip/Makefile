TOPDIR=		../../../

include $(TOPDIR)/misc/mk/config.mk
include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=  someip.lo

LO_OBJS=   	someip-sd.o \
			someip-lists.o \
			someip-sock.o \



all: $(LO_TARGET)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk

