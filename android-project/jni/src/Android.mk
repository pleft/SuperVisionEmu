LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_SRC_FILES := platform/SDL/main.c \
 common/controls.c \
 common/gpu.c \
 common/interrupts.c \
 common/memorymap.c \
 common/sound.c \
 common/timer.c \
 common/version.c \
 common/watara.c \
 common/io.c \
 common/log.c \
 common/memory.c \
 common/M6502/M6502.c \

LOCAL_STATIC_LIBRARIES := SDL2_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,SDL)LOCAL_PATH := $(call my-dir)
