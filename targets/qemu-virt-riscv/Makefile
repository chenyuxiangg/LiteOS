include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

USER_SRC :=  $(wildcard  $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/src/*.c)
USER_SRC +=  $(wildcard  $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/os_adapt/*.c)
USER_SRC += $(wildcard  $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/src/*.S)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/Hifive1_Revb_Firmware/freedom-metal \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/uart/include/ext \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/uart/include/hal

BOARD_INCLUDES += $(BSP_INC)

C_DEFS =  \
    -DUSE_STDPERIPH_DRIVER

BOARD_SRCS += $(USER_SRC)
BOARD_DEF += $(C_DEFS)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_NO_WARNING := -Wno-misleading-indentation

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LOCAL_NO_WARNING)

include $(MODULE)
