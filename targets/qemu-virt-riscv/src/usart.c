/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-08-04
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "hal_ns16550.h"
#include "los_hwi.h"
#include "platform.h"

VOID UartInit(VOID)
{
    UART_PARAMS params = {
        .parity_en = 0,
        .parity_force_en = 0,
        .stop_bit = 0,
        .data_len = UART_DL_8
    };
    hal_uart_set_params(params);
    hal_uart_set_baudrate(UART_BAUDRATE);
}

VOID UsartWrite(const CHAR c)
{
    hal_uart_putc(c);
}

UINT8 UsartRead(VOID)
{
    CHAR ch;
    hal_uart_getc(&ch);
    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
}

INT32 UsartHwi(VOID)
{
    UINT32 ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 1, 0, UartHandler, NULL);
    if (ret != LOS_OK) {
        dprintf("%s, %d, ret = %#x\n", __FUNCTION__, __LINE__, ret);
    }
    ret = LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);
    if (ret != LOS_OK) {
        dprintf("%s, %d, ret = %#x\n", __FUNCTION__, __LINE__, ret);
    }
    hal_uart_set_irq(UART_IRQ_RX_READY, 1);
    return LOS_OK;
}

UartControllerOps g_genericUart = {
    .uartInit = UartInit,
    .uartWriteChar = UsartWrite,
    .uartReadChar = UsartRead,
    .uartHwiCreate = UsartHwi
};
