#include "platform.h"
#include "hal_ns16550.h"

#define READ_BYTE(addr)             ({unsigned char val = *(volatile unsigned char *)(addr);val;})
#define WRITE_BYTE(addr, val)       (*(volatile unsigned char *)(addr)=val)
#define CLEAN_BIT(val, bit)         ({val=(~(1 << bit)) & val;val;})
#define SET_BIT(val, bit)           ({val=((1 << bit) | val);val;})

static unsigned int hal_get_real_datalen_from_param(const UART_PARAMS params)
{
    return (params.data_len + UART_DATA_LEN_BASE);
}

void hal_uart_set_params(const UART_PARAMS params)
{
    hal_uart_set_parity_en(params.parity_en);
    hal_uart_set_parity_mode((HAL_PARITY_MODE)params.parity_mode);
    hal_uart_set_parity_force_en(params.parity_force_en);
    hal_uart_set_stop_bit(params.stop_bit);
    hal_uart_set_data_len(hal_get_real_datalen_from_param(params));
}

void hal_uart_set_baudrate(unsigned int baudrate)
{
    unsigned char psd = 0;
    unsigned char dl = (UART0_CLOCK) / (16 * baudrate) / (psd + 1);

    WRITE_BYTE((UART0_BASE_ADDR + LCR), (1 << DLAB));
    WRITE_BYTE((UART0_BASE_ADDR + DLL), (dl & 0xff));
    WRITE_BYTE((UART0_BASE_ADDR + DLM), ((dl >> 8) & 0xff));
    // WRITE_BYTE((UART0_BASE_ADDR + PSD), psd);
    WRITE_BYTE((UART0_BASE_ADDR + LCR), 0x0);
}

void hal_uart_set_parity_en(unsigned char en)
{
    unsigned char lcr = READ_BYTE(UART0_BASE_ADDR + LCR);
    if (en == 0) {
        CLEAN_BIT(lcr, ENABLE_PARITY);
    } else if (en == 1) {
        SET_BIT(lcr, ENABLE_PARITY);
    } else {
        return;
    }
    WRITE_BYTE((UART0_BASE_ADDR + LCR), lcr);
}

void hal_uart_set_irq_en(unsigned char en)
{
    WRITE_BYTE((UART0_BASE_ADDR + IER), (en & 0xff));
}

void hal_uart_set_parity_mode(HAL_PARITY_MODE mode)
{
    unsigned char lcr = READ_BYTE(UART0_BASE_ADDR + LCR);
    if (mode == PARITY_ODD) {
        CLEAN_BIT(lcr, EVEN_PARITY);
    } else if (mode == PARITY_EVEN) {
        SET_BIT(lcr, EVEN_PARITY);
    } else {
        return;
    }
    WRITE_BYTE((UART0_BASE_ADDR + LCR), lcr);
}

void hal_uart_set_parity_force_en(unsigned char en)
{
    unsigned char lcr = READ_BYTE(UART0_BASE_ADDR + LCR);
    if (en == 0) {
        CLEAN_BIT(lcr, FORCE_PARITY);
    } else if (en == 1) {
        SET_BIT(lcr, FORCE_PARITY);
    } else {
        return;
    }
    WRITE_BYTE((UART0_BASE_ADDR + LCR), lcr);
}

void hal_uart_set_stop_bit(unsigned char bit_num)
{
    unsigned char lcr = READ_BYTE(UART0_BASE_ADDR + LCR);
    if (bit_num == 0) {
        CLEAN_BIT(lcr, STOP_BITS);
    } else if (bit_num == 1) {
        SET_BIT(lcr, STOP_BITS);
    } else {
        return;
    }
    WRITE_BYTE((UART0_BASE_ADDR + LCR), lcr);
}

void hal_uart_set_data_len(unsigned char data_len)
{
    unsigned char lcr = READ_BYTE(UART0_BASE_ADDR + LCR);
    CLEAN_BIT(lcr, WORD_LEN0);
    CLEAN_BIT(lcr, WORD_LEN1);
    switch (data_len) {
        case 6:
            SET_BIT(lcr, WORD_LEN0);
            break;
        case 7:
            SET_BIT(lcr, WORD_LEN1);
            break;
        case 8:
            SET_BIT(lcr, WORD_LEN0);
            SET_BIT(lcr, WORD_LEN1);
            break;
        case 5:
        default:
            break;
    }
    WRITE_BYTE((UART0_BASE_ADDR + LCR), lcr);
}

void hal_uart_set_irq(UART_IRQ_TYPE type, unsigned char en)
{
    unsigned char ier = READ_BYTE(UART0_BASE_ADDR + IER);
    if (en == 0) {
        CLEAN_BIT(ier, type);
    } else {
        SET_BIT(ier, type);
    }
    WRITE_BYTE((UART0_BASE_ADDR + IER), ier);
}

void hal_uart_putc(unsigned char ch)
{
    while ((READ_BYTE(UART0_BASE_ADDR + LSR) & LSR_TX_VALID) == 0);
	WRITE_BYTE((UART0_BASE_ADDR + THR), ch);
}

int hal_uart_write(const char *s, int len, int timeout)
{
    (void)timeout;
    for (int i = 0; i < len; ++i) {
        hal_uart_putc(s[i]);
    }
    return len;
}

void hal_uart_getc(char *ch)
{
    if ((READ_BYTE(UART0_BASE_ADDR + LSR) & LSR_RX_VALID) == 0) {
        return;
    }
	*ch = READ_BYTE(UART0_BASE_ADDR + RHR);
}