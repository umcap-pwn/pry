# ------------------------------------------------------------------------------------ #
# This file is the part of the source code and may be overwritten with the next pull.  #
# If you want to safely tewak the build options, use config.mk.local instaed.          #
# ------------------------------------------------------------------------------------ #

# --- CODE TWEAKS ---

# Lower size -> lower memory consumption, but more reallocs
STRBUF_INIT_CAP ?= 64


# --- END OF CODE TWEAKS SECTION ---
# In most cases you don't want to change anything under
CPPFLAGS += -DSTRBUF_INIT_CAP=$(STRBUF_INIT_CAP)

# COMPILER TWEAKS
VERSION  = 0.1.0
PREFIX   = /usr/local
DESTDIR  =

CC       = cc
# -Wpedantic
CFLAGS  = -std=c11 -O3 -g -Wall -Wextra  -Wshadow -Wconversion \
          -Wformat=2 -Wformat-security -Wnull-dereference \
          -fstack-protector-strong -fstack-clash-protection \
          -fcf-protection=full -fPIE -flto

CPPFLAGS = -Iinclude -DVERSION=\"$(VERSION)\" \
           -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 \
           -D_POSIX_C_SOURCE=200809L

LDFLAGS = -pie -Wl,-z,relro,-z,now -Wl,-z,noexecstack -Wl,-z,defs -flto

ifdef DEBUG
	CFLAGS += -O0 -fno-omit-frame-pointer -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif
