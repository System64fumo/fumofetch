CFLAGS=-Oz -s -mtune=native -march=native
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

all: fumofetch

install: $(BINS)
	@echo "Installing..."
	@install -D -t $(DESTDIR)$(BINDIR) fumofetch

clean:
	rm -f fumofetch logo.h

fumofetch: logo.h main.c functions.c
	@echo "Compiling the fumo..."
	@$(CC) -o fumofetch main.c functions.c $(CFLAGS)

logo.h: logo.txt
	@echo "Generating logo..."
	@echo "#ifndef LOGO_H" > logo.h
	@echo "#define LOGO_H" >> logo.h
	@echo "#define LOGO_HEIGHT" `wc -l logo.txt | cut -d ' ' -f 1` >> logo.h
	@echo >> logo.h
	@echo "static const char* logo_array[] = {" >> logo.h
	@while IFS= read -r line; do \
		escaped_line=$$(echo "$$line" | sed 's/"/\\"/g' | sed 's/\\/\\\\/g'); \
		echo "	\"$$escaped_line\"," >> logo.h; \
	done < logo.txt
	@echo "	NULL" >> logo.h
	@echo "};" >> logo.h
	@echo >> logo.h
	@echo "#endif" >> logo.h
