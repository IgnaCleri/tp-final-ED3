#include "LPC17xx.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpdma.h"
#include "audio_data.h"


void conf_dac(void);
void conf_lli(void);
void conf_gpdma(void);

GPDMA_LLI_T LLI0 = {0};
GPDMA_LLI_T LLI1 = {0};
GPDMA_LLI_T LLI2 = {0};

int main(void)
{

	conf_dac();
	conf_lli();
	conf_gpdma();

	while(1)
	{

		__WFI();

	}

}

void conf_lli(void)
{

	LLI1.srcAddr = (uint32_t)& (audio_samples[4095]);
	LLI1.dstAddr = (uint32_t)& LPC_DAC->DACR;
	LLI1.nextLLI = (uint32_t)& LLI2;
	LLI1.control = 4095 | 1<<18 | 1<<21 | 1<<26;

	LLI0.srcAddr = (uint32_t)& (audio_samples[0]);
	LLI0.dstAddr = (uint32_t)& LPC_DAC->DACR;
	LLI0.nextLLI = (uint32_t)& LLI1;
	LLI0.control = 4095 | 1<<18 | 1<<21 | 1<<26;

	LLI2.srcAddr = (uint32_t)& (audio_samples[8190]);
	LLI2.dstAddr = (uint32_t)& LPC_DAC->DACR;
	LLI2.nextLLI = (uint32_t)& LLI0;
	LLI2.control = 2438 | 1<<18 | 1<<21 | 1<<26;
}

void conf_dac(void)
{

	DAC_CONVERTER_CFG_T dactimconf =
	{
		.doubleBuffer = ENABLE,
		.dmaCounter = ENABLE,
		.dmaRequest = ENABLE
	};

	DAC_Init();
	DAC_SetBias(DAC_350uA);
	DAC_SetDMATimeOut(3125);
	DAC_ConfigDAConverterControl(&dactimconf);


}


// 2 de 4095 + 1 de 2438
void conf_gpdma(void)
{

	const GPDMA_Endpoint_T endpts =
	{

		.increment = ENABLE,
		.burst = GPDMA_BSIZE_1,
		.width = GPDMA_HALFWORD

	};

	const GPDMA_Endpoint_T endptd =
		{

			.increment = DISABLE,
			.burst = GPDMA_BSIZE_1,
			.width = GPDMA_HALFWORD

		};

	GPDMA_Channel_CFG_T confaudio =
	{
		.channelNum = GPDMA_CH_3,
		.transferSize = 4095,
		.type = GPDMA_M2P,
		.dstConn = GPDMA_DAC,
		.srcMemAddr = (uint32_t)& audio_samples[0],
		.dstMemAddr = 0,
		.src = endpts,
		.dst = endptd,
		.intTC = DISABLE,
		.intErr = DISABLE,
		.linkedList = (uint32_t)& LLI1
	};

	GPDMA_Init();
	GPDMA_SetupChannel(&confaudio);



}
