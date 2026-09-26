include config.mk

SRCS = $(wildcard src/*.c src/elf/*.c src/analysis/*.c src/cmd/*.c src/util/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# For now libcapstone that is needed for disasm module is an external dependency.
# Might change in the future, but for now pry neds libcapstone on your system to build.
PKGCONF  = pkg-config
PKGDEPS  = capstone
CFLAGS   += -I$(shell $(PKGCONF) --cflags $(PKGDEPS))
LDFLAGS  += $(shell $(PKGCONF) --libs $(PKGDEPS))

# Hack to search for deps and fail with nice error if not found.
# Without this, it will fail with cryptic "package.h not found".
_ =
$(foreach p,$(PKGDEPS),$(if $(shell $(PKGCONF) --exists $(p) && echo 1),,\
$(error Required library not found: $(p) — install it or set PKG_CONFIG_PATH)))

pry: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) pry

install: pry
	install -Dm755 pry $(DESTDIR)$(PREFIX)/bin/pry

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/pry

re:
	make clean && make pry

# Optional target, configures the environment for development.
# You will need bear installed to generate compile_commands.json.
config:
	bear -- make re

.PHONY: all clean install uninstall e

# Apply any user-defined configs
-include config.local.mk
