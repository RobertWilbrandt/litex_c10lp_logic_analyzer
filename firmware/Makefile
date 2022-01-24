BUILD_ROOT_DIR=../build
BUILD_DIR=$(BUILD_ROOT_DIR)/firmware
SRC_DIR=src
SOFTWARE_DIR=$(BUILD_ROOT_DIR)/soc/software

include $(BUILD_ROOT_DIR)/soc/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

OBJECT_NAMES = isr.o main.o crt0.o libc_syscalls.o
OBJECTS = $(OBJECT_NAMES:%=$(BUILD_DIR)/%)

COMMONFLAGS = $(DEPFLAGS) -Os $(CPUFLAGS) -g3 -fomit-frame-pointer -Wall -fno-stack-protector -flto -I$(LIBBASE_DIRECTORY) -I$(SOC_DIRECTORY)/software/include -I$(SOC_DIRECTORY)/software -I$(CPU_DIRECTORY) -I$(BUILDINC_DIRECTORY) -nostartfiles
CFLAGS = $(COMMONFLAGS) -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes
CXXFLAGS = $(COMMONFLAGS) -std=c++17
LDFLAGS = --specs=nano.specs --specs=nosys.specs $(CFLAGS)

all: $(BUILD_DIR)/firmware.bin

clean:
	rm -rf $(BUILD_DIR)

# See https://ismail.badawi.io/blog/automatic-directory-creation-in-make/
.PRECIOUS: $(BUILD_DIR)/.

$(BUILD_DIR)/.:
	mkdir -p $@

-include $(OBJECTS:.o=.d)

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@
	@chmod -x $@

$(BUILD_DIR)/%.elf: $(OBJECTS)
	$(CX) $(LDFLAGS) \
	  -T linker.ld \
	  -N -o $@ \
	  $(OBJECTS) \
	  $(PACKAGES:%=-L$(SOFTWARE_DIR)/%) \
	  $(LIBS:lib%=-l%) \
	  -lstdc++
	@chmod -x $@

.SECONDEXPANSION:

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $$(@D)/.
	$(CX) -c $(CXXFLAGS) $(1) $< -o $@

$(BUILD_DIR)/crt0.o: $(CPU_DIRECTORY)/crt0.S | $$(@D)/.
	$(CC) -c $(CFLAGS) -o $@ $<
