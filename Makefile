# 

TARGET := main

DIRECTORIES := Camera DataStructure Demo GtkCustomWidget GtkStuff Math Rendering


SRCDIRS := . $(addprefix ./Sources/, $(DIRECTORIES))

CFILES := $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
HFILES := $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.h))
OFILES := $(patsubst %.c, %.o, $(CFILES))
DFILES := $(patsubst %.h, %.d, $(HFILES))

DEPINC := $(foreach D, $(SRCDIRS), -I$(D))

CC := gcc
LINKER := gcc

GTKCFLAGS := `pkg-config gtk4 --cflags`
GTKLFLAGS := `pkg-config gtk4 --libs`

EPOXYCFLAGS := `pkg-config epoxy --cflags`
EPOXYLFLAGS := `pkg-config epoxy --libs`

OTHERLFLAGS := -lm -lGL

DEPFLAGS := -MP -MD

CFLAGS := -Wall -O2 $(CCOND) $(DEPFLAGS) $(DEPINC) $(GTKCFLAGS) $(EPOXYCFLAGS)
LFLAGS := -Wall -no-pie $(OTHERLFLAGS) $(GTKLFLAGS) $(EPOXYLFLAGS)

all : $(TARGET)

$(TARGET) : $(OFILES)
	@$(LINKER) $(OFILES) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OFILES) : %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean :
	@rm -rf $(TARGET) $(OFILES) $(DFILES)
	@echo "Clean up completed!"

run : $(TARGET)
	./$(TARGET)

-include $(DFILES)
