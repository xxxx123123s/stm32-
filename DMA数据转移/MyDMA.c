#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;					//定义全局变量，用于记住Init函数的Size，供Transfer函数使用

/**
  * 函    数：DMA初始化
  * 参    数：AddrA 原数组的首地址
  * 参    数：AddrB 目的数组的首地址
  * 参    数：Size 转运的数据大小（转运次数）
  * 返 回 值：无
  */
void MyDMA_Init(uint32_t ADD_A, uint32_t ADD_B, uint16_t Size)
{
	MyDMA_Size = Size;					//将Size写入到全局变量，记住参数Size
	
	/*开启时钟*/




    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitTypeDef MyDMA_structurn;
	
     
	
	
	MyDMA_structurn.DMA_BufferSize = Size;
    MyDMA_structurn.DMA_DIR = DMA_DIR_PeripheralSRC;
    MyDMA_structurn.DMA_M2M = DMA_M2M_Enable;
    MyDMA_structurn.DMA_MemoryBaseAddr = ADD_B;
    MyDMA_structurn.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    MyDMA_structurn.DMA_MemoryInc = DMA_MemoryInc_Enable;
    MyDMA_structurn.DMA_Mode = DMA_Mode_Normal;
    MyDMA_structurn.DMA_PeripheralBaseAddr = ADD_A;
    MyDMA_structurn.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    MyDMA_structurn.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    MyDMA_structurn.DMA_Priority = DMA_Priority_Medium;
    
    
    
    
    DMA_Init(DMA1_Channel1, &MyDMA_structurn);

//DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);写入配置
    
    DMA_Cmd(DMA1_Channel1, DISABLE );//初始化前关闭 → 写入配置 → 需要传输时再DMA_Cmd(ENABLE)启动搬运。


}

/**
  * 函    数：启动DMA数据转运
  * 参    数：无
  * 返 回 值：无
  */
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);					//DMA失能，在写入传输计数器之前，需要DMA暂停工作
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);	//写入传输计数器，指定将要转运的次数
	DMA_Cmd(DMA1_Channel1, ENABLE);						//DMA使能，开始工作
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	//等待DMA工作完成
	DMA_ClearFlag(DMA1_FLAG_TC1);						//清除工作完成标志位
}
