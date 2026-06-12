local := $(testtree)

tests-prefix := tests

# ------------------------------------------------------------------

# -------------------------
slave-queue-objs := pop.o init.o push.o

slave-queue-srctree := $(srctree)/slave/src/queue
slave-queue-objtree := $(objtree)/$(tests-prefix)/slave/queue
# -------------------------

slave-tests-prefix := slave
slave-tests := queue.test

slave-complete-prefix := $(objtree)/$(tests-prefix)/$(slave-tests-prefix)

# ------------------------------------------------------------------

master-tests-prefix := master
master-tests := 

master-complete-prefix := $(objtree)/$(tests-prefix)/$(master-tests-prefix)


# =============================================

phony += all
all: slave master


# =============================================

phony += master
master: cflags += -I$(srctree)/master/include
master: $(master-complete-prefix) $(addprefix $(master-complete-prefix)/, $(master-tests))

$(master-complete-prefix):
	$(q)mkdir -p $@

$(master-complete-prefix)/%: $(local)/$(master-tests-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $(basename $@).test.o
	$(q)$(ld) $(ldflags) $(basename $@).test.o -o $@ $(addprefix -l, $(libs))


# =============================================

phony += slave
slave: cflags += -I$(srctree)/slave/include
slave: $(slave-complete-prefix) $(addprefix $(slave-complete-prefix)/, $(slave-tests))

$(slave-complete-prefix):
	$(q)mkdir -p $@

$(slave-complete-prefix)/%: required-objs := $(addprefix $(slave-$(basename %)-objtree), $(slave-$(basename %)-objs))
$(slave-complete-prefix)/%: $(required-objs) $(local)/$(slave-tests-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $(basename $@).test.o
	$(q)$(ld) $(ldflags) $(basename $@).test.o -o $@ $(addprefix -l, $(libs))

$(slave-%-objtree):
	$(q)mkdir -p $@
	


# =============================================

.PHONY: $(phony)
