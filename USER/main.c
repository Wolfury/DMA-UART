#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "dma.h"


//ALIENTEK ̽����STM32F407������ ʵ��23
//DMA ʵ��-�⺯���汾  
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

const u8 TEXT_TO_SEND[]={"STM32F4 DMA ����ʵ��"};	 
u8 temp=0;
  
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
/* 	//���г������ʵ�����ڴ浽�����DMA����
	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	j=sizeof(TEXT_TO_SEND);	   
	for(i=0;i<SEND_BUF_SIZE;i++)//���ASCII�ַ�������
    {
		if(t>=j)//���뻻�з�
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
		}else//����TEXT_TO_SEND���
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
		//�ȴ�DMA������ɣ���ʱ������������һЩ�£����
		//ʵ��Ӧ���У����������ڼ䣬����ִ�����������
		while(1)
		{	
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����     
			MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);     //��ʼһ��DMA���䣡	 			
			while(1)
			{
				if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
				{ 
					delay_ms(5);
					DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־���ܹؼ�����������ֻ�ܷ���һ��
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
	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)USART_RX_BUF,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
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
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
	}
	MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); 	//��ͨģʽ��ÿ�ζ�ȡ���һ�ζ�Ҫ����ʹ���������ڶ���
												//ѭ��ģʽ�²���Ҫ����ʹ��
}

