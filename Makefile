# Project Name
TARGET = MuxTest

DEBUG = 1

# Sources
CPP_SOURCES = MuxTest.cpp QSPI_Settings.cpp

# Library Locations
LIBDAISY_DIR = ../DaisyExamples/libdaisy
DAISYSP_DIR = ../DaisyExamples/DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

