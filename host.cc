/*
 * Program:

 *   This program is a C++ driver for using one DMA in ZCU102 board.
 *
 * History:
 *   2020/5/14     Yuanlong Xiao   First Release
*/
#include "config.h"
#include "typedefs.h"



int main(void)
{
	int Status;

	DmaToPL dma_send(DMA_DEV_ID_0,
			 MAX_PKT_LEN,
			 TX_BD_SPACE_HIGH,
			 TX_BD_SPACE_BASE,
			 NUMBER_OF_PACKETS,
			 TX_BUFFER_BASE);

	DmaFromPL dma_recv(DMA_DEV_ID_1,
			 RX_BD_SPACE_HIGH,
			 RX_BD_SPACE_BASE,
			 MAX_PKT_LEN_RX,
			 NUMBER_OF_PACKETS_RX,
			 RX_BUFFER_BASE);

	Status = dma_send.dma_init();
	if (Status != XST_SUCCESS) {
		xil_printf("Device Initiation Failed!\r\n");
		return XST_FAILURE;
	}

	Status = dma_recv.dma_init();
	if (Status != XST_SUCCESS) {
		xil_printf("Device Initiation Failed!\r\n");
		return XST_FAILURE;
	}


	Status = dma_recv.RecvPackets();
		if (Status != XST_SUCCESS) {
			xil_printf("Receiving Packets Failed!\r\n");
			return XST_FAILURE;
	}


	Status = dma_send.WR2TxBuffer();
	if (Status != XST_SUCCESS) {
		xil_printf("Write Buffer Failed!\r\n");
		return XST_FAILURE;
	}

	Status = dma_send.SendPackets();
	if (Status != XST_SUCCESS) {
		xil_printf("Sending Packets Failed!\r\n");
		return XST_FAILURE;
	}

	Status = dma_send.CleanTxBuffer();
	if (Status != XST_SUCCESS) {
		xil_printf("Cleaning TxBuffer Failed!\r\n");
		return XST_FAILURE;
	}

	Status = dma_recv.RecvWait();
	if (Status != XST_SUCCESS) {
		xil_printf("Receiving Waits Failed!\r\n");
		return XST_FAILURE;
	}

	double Total_time = ((double)(dma_recv.EndTotal - dma_send.StartTotal) / (COUNTS_PER_SECOND / 1000000));
		printf("Overall Time: %.2lfus\n", Total_time);

	Status = dma_recv.CheckData();
	if (Status != XST_SUCCESS) {
		xil_printf("Checking Data Failed!\r\n");
		return XST_FAILURE;
	}

	Status = dma_recv.CleanRxBuffer();
	if (Status != XST_SUCCESS) {
		xil_printf("Cleaning RxBuffer Failed!\r\n");
		return XST_FAILURE;
	}

	xil_printf("Everything is perfect\n");
	return XST_SUCCESS;
}


