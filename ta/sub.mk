global-incdirs-y += include
#global-incdirs-y += ../host/include
srcs-y += my_test.c
srcs-y += my_test_handle.c
srcs-y += normrnd.c
srcs-y += tee_math.c
srcs-y += GS.c

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
