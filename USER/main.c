#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "dma.h"


//ALIENTEK 探索者STM32F407开发板 实验23
//DMA 实验-库函数版本  
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

const u8 TEXT_TO_SEND[]={"STM32F4 DMA 串口实验"};	 
u8 temp=0;
  
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
/* 	//下列程序可以实现由内存到外设的DMA传输
	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	j=sizeof(TEXT_TO_SEND);	   
	for(i=0;i<SEND_BUF_SIZE;i++)//填充ASCII字符集数据
    {
		if(t>=j)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}else 
			{
				SendBuff[i]=0x0d;
				mask++;
			}	
		}else//复制TEXT_TO_SEND语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}   	   
    }		 
	i=0;
	while(1)
	{
		printf("\r\nDMA DATA:\r\n"); 	    
		//等待DMA传输完成，此时我们来做另外一些事，点灯
		//实际应用中，传输数据期间，可以执行另外的任务
		while(1)
		{	
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);     //开始一次DMA传输！	 			
			while(1)
			{
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
				{ 
					delay_ms(5);
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志，很关键，如果不清除只能发送一次
					break; 
				}	
				i++;
			}
			printf("DMA runs for %d\n",i);
			printf("t=%d\n",temp);
			i=0;
			delay_ms(1000);
		}			    
	}
*/	
	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)USART_RX_BUF,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); 
	MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); 
	while(1)
	{
		temp++;
	}
	
}

void DMA2_Stream7_IRQHandler(void)
{
	printf("IT!\r\n");
	printf("temp=%d\r\n",temp);
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7) != RESET)
	{
		printf("DMA transfer finished!\r\n");
		printf("the data is %d\r\n",USART_RX_BUF[0]);
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
	}
	MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); 	//普通模式下每次读取完成一次都要重新使能来开启第二次
												//循环模式下不需要重新使能
}

