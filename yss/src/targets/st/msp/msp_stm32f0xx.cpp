////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License V3.3
//
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
//
// 본 소스 코드를 :
//		- 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
//		- 상업적 또는 비 상업적 이용이 가능합니다.
//		- 본 저작권 표시 주석을 제외한 코드의 내용을 임의로 수정하여 사용하는 것은 허용합니다.
//		- 사용자가 수정한 코드를 사용자의 고객사에게 상호간 전달은 허용합니다.
//		- 그러나 수정하여 다수에게 재배포하는 행위를 금지합니다. 
//		- 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//		- 어떤 형태의 기여든지, 그것은 기증으로 받아들입니다.
//
// 본 소스 코드는 프리웨어로 앞으로도 유료로 전환하지 않을 것입니다.
// 사용자 또는 부품의 제조사가 요구하는 업데이트가 있을 경우 후원금을 받아 
// 요구하는 사항을 업데이트 할 예정입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2024. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <drv/peripheral.h>

#if defined(STM32F0)

#include <config.h>
#include <yss/instance.h>
#include <targets/st/bitfield.h>

void __WEAK initializeSystem(void)
{
	// Power Control 장치 활성화
	clock.enableApb1Clock(RCC_APB1ENR_PWREN_Pos);

	// 외부 고속 클럭 활성화
#if defined(HSE_CLOCK_FREQ)
	clock.enableHse(HSE_CLOCK_FREQ);
#endif

	using namespace define::clock;

	// 주 PLL 활성화
	// pllClock = HSE_CLOCK_FREQ * (mul + 2) / (1 + xtpre);
#if HSE_CLOCK_FREQ == 8000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		0,				// uint8_t xtpre;
		7				// uint8_t mul;
	);
#define PLL_ENABLED
# elif HSE_CLOCK_FREQ == 12000000
	clock.enableMainPll(
		pll::src::HSE,	// uint8_t src;
		0,				// uint8_t xtpre;
		2				// uint8_t mul;
	); 
#define PLL_ENABLED
#endif

#if defined(PLL_ENABLED)
	flash.setLatency(48000000);

	// 시스템 클럭 설정
	clock.setSysclk(
		sysclk::src::PLL,		// uint8_t sysclkSrc;
		divFactor::ahb::NO_DIV, // uint8_t ahb;
		divFactor::apb::NO_DIV,	// uint8_t apb1;
		divFactor::apb::NO_DIV	// <- 사용 안됨
	);
#endif

	flash.setPrefetchEn(true);

	// GPIO 활성화
	clock.enableAhb1Clock(RCC_AHBENR_GPIOAEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOBEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOCEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIODEN_Pos);
	clock.enableAhb1Clock(RCC_AHBENR_GPIOFEN_Pos);
}

void initializeDma(void)
{
	// DMA1
	dmaChannel1.enableClock();
	dmaChannel1.initialize();
	dmaChannel1.enableInterrupt();
	dmaChannel2.initialize();
	dmaChannel2.enableInterrupt();
	dmaChannel3.initialize();
	dmaChannel3.enableInterrupt();
	dmaChannel4.initialize();
	dmaChannel4.enableInterrupt();
	dmaChannel5.initialize();
	dmaChannel5.enableInterrupt();
}


#endif