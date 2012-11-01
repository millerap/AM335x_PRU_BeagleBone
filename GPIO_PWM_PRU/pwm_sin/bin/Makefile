-include DIRS

CROSS_COMPILE?=arm-arago-linux-gnueabi-
LIBDIR_APP_LOADER?=../lib
INCDIR_APP_LOADER?=../include
BINDIR_APPLICATIONS?=../bin
BINDIR_FW?=./
PASM?=../utils/pasm

all:
# Make PRU example applications
#	mkdir -p bin

#	for dir in $(APP_DIRS); do make -C $$dir CROSS_COMPILE="${CROSS_COMPILE}" LIBDIR_APP_LOADER="${LIBDIR_APP_LOADER}" LIBDIR_EDMA_DRIVER="${LIBDIR_EDMA_DRIVER}" INCDIR_APP_LOADER="${INCDIR_APP_LOADER}" INCDIR_EDMA_DRIVER="${INCDIR_EDMA_DRIVER}" BINDIR="${BINDIR_APPLICATIONS}"; done

# Pass PRU assembly code for each example through assembler
#	for a_file in ${ASSEM_FILES} ; \
#	do \
#          ${PASM} -b $$a_file ; \
#	done ; \
#	mv *.bin ${BINDIR_FW}

	${PASM} -b blinker.p

clean:
	for dir in $(APP_DIRS); do make -C $$dir clean LIBDIR_APP_LOADER="${LIBDIR_APP_LOADER}" LIBDIR_EDMA_DRIVER="${LIBDIR_EDMA_DRIVER}" INCDIR_APP_LOADER="${INCDIR_APP_LOADER}" INCDIR_EDMA_DRIVER="${INCDIR_EDMA_DRIVER}" BINDIR="${BINDIR_APPLICATIONS}"; done

	for bin_file in ${BIN_FILES}; do rm -fr ${BINDIR_FW}/$$bin_file; done

