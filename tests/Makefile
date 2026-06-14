local := $(testtree)

tests-prefix := tests

# ------------------------------------------------------------------

slave-queue-objs := queue/pop.o queue/init.o queue/push.o ram/copy.o

slave-format_string-objs := ram/copy.o ram/format_string.o arch/dummy/dummy/math/divmod10.o

slave-srctree := $(srctree)/slave/src
slave-objtree := $(objtree)/$(tests-prefix)/slave

slave-tests-prefix := slave
slave-tests := \
	queue.test \
	format_string.test

slave-complete-prefix := $(objtree)/$(tests-prefix)/$(slave-tests-prefix)

# ------------------------------------------------------------------

master-tests-prefix := master
master-tests := 

master-complete-prefix := $(objtree)/$(tests-prefix)/$(master-tests-prefix)


# =============================================

phony += all
all: slave master


# =============================================

cflags += -I$(cmocka-include) \
	-I$(gentree) -I$(srctree)/include \
	$(patsubst "%",%,$(CONFIG_OPT_LEVEL))

ldflags += -L$(cmocka-libdir)

libs += cmocka

# нам спецом нужно пересобирать некоторые файлы из прошивок,
# чтобы потом можно было их слинковать с тестами.
# для этого для master и slave есть переменная build-for-tests,
# который при y собирает только необходимые для тестов файлы.
# результат хранится отдельно под префиксом $(objtree)/$(tests-prefix)
export build-for-tests = y


# =============================================

phony += master
master: cflags += -I$(srctree)/master/include
master: build-master-for-tests $(master-complete-prefix) $(addprefix execute/, $(master-tests))

phony += build-master-for-tests
build-master-for-tests:
	$(q)$(MAKE) -C $(srctree)/master

$(master-complete-prefix):
	$(q)mkdir -p $@

# см. https://stackoverflow.com/questions/9007039/makefile-execute-phonytargets-of-a-file-only-if-file-does-not-exists
# https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_node/make_98.html#SEC97
.INTERMEDIATE: $(addprefix execute/, $(master-tests))
execute/%: $(master-complete-prefix)/%
	$(q)cp $< $(objtree)/$(tests-prefix)
	$(q)LD_PRELOAD=$(cmocka-lib) $<

# выглядит уродливо, но пока так
$(master-complete-prefix)/%: $(master-complete-prefix)/%.o
	$(q)$(ld) $(ldflags) $(addprefix $(master-objtree)/, $(master-$(basename $(notdir $@))-objs)) $< -o $@ $(addprefix -l, $(libs))

$(master-complete-prefix)/%.o: $(local)/$(master-tests-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $@


# =============================================

phony += slave
slave: cflags += -I$(srctree)/slave/include
slave: build-slave-for-tests $(slave-complete-prefix) $(addprefix execute/, $(slave-tests))

phony += build-slave-for-tests
build-slave-for-tests:
	$(q)$(MAKE) -C $(srctree)/slave

$(slave-complete-prefix):
	$(q)mkdir -p $@

.INTERMEDIATE: $(addprefix execute/, $(slave-tests))
execute/%: $(slave-complete-prefix)/%
	$(q)cp $< $(objtree)/$(tests-prefix)
	$(q)LD_PRELOAD=$(cmocka-lib) $<

$(slave-complete-prefix)/%: $(slave-complete-prefix)/%.o
	$(q)$(ld) $(ldflags) $(addprefix $(slave-objtree)/, $(slave-$(basename $(notdir $@))-objs)) $< -o $@ $(addprefix -l, $(libs))

$(slave-complete-prefix)/%.o: $(local)/$(slave-tests-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $@


# =============================================

.PHONY: $(phony)
