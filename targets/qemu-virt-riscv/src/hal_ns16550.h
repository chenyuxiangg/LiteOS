#ifndef __HAL_NS16550_H__
#define __HAL_NS16550_H__

/*
* THR: TX FIFO
* RHR: RX FIFO
* LSR: RX STATUS
* DLL: DL寄存器的低8bit,用于配置波特率
* DLM: DL寄存器的高8bit,用于配置波特率
* PSD: 用于配置波特率
* DLAB: LCR的一个字段，用于将复用关系选择为DLL、DLM
*/
#define THR     0x0
#define RHR     0x0
#define IER     0x1
#define LSR     0x5
#define LCR     0x3
#define DLL     0x0
#define DLM     0x1
#define PSD     0x5

/* ======== Register bit define ======== */
#define DLAB            7
#define EVEN_PARITY     4
#define ENABLE_PARITY   3
#define FORCE_PARITY    5
#define STOP_BITS       2
#define WORD_LEN0       0
#define WORD_LEN1       1

/* ======== FIFO State ======== */
#define LSR_TX_VALID     (1 << 5)
#define LSR_RX_VALID     (1 << 0)

typedef enum {
    PARITY_ODD = 0,
    PARITY_EVEN
} HAL_PARITY_MODE;

#define UART_BAUDRATE       (9600)
#define UART_DATA_LEN_BASE  (5)

typedef enum {
    UART_DL_5 = 0,
    UART_DL_6 = 1,
    UART_DL_7 = 2,
    UART_DL_8 = 3
} UART_DATA_LENGTH;

typedef struct {
    unsigned char parity_en : 1;
    unsigned char parity_mode : 1;
    unsigned char parity_force_en : 1;
    unsigned char stop_bit : 1;
    unsigned char data_len : 2;
    unsigned char rsv : 2;
} UART_PARAMS;

typedef enum {
    UART_IRQ_RX_READY=0,
    UART_IRQ_TX_EMPTY=1,
    UART_IRQ_RX_ERROR=2,
    UART_IRQ_MODLE_STATUS=3,
    UART_IRQ_DMA_RX=6,
    UART_IRQ_DMA_TX=7,
} UART_IRQ_TYPE;

void hal_uart_set_params(const UART_PARAMS params);
void hal_uart_set_baudrate(unsigned int baudrate);
void hal_uart_set_parity_en(unsigned char en);
void hal_uart_set_parity_mode(HAL_PARITY_MODE mode);
void hal_uart_set_parity_force_en(unsigned char en);
void hal_uart_set_stop_bit(unsigned char bit_num);
void hal_uart_set_data_len(unsigned char data_len);
int hal_uart_write(const char *s, int len, int timeout);
void hal_uart_putc(unsigned char ch);
void hal_uart_set_irq(UART_IRQ_TYPE type, unsigned char en);
void hal_uart_getc(char *ch);

#endif