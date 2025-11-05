/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ff.h"
#include "keyled.h"
#include "w25flash.h"
#include "file_opera.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  // Start Menu
  uint8_t startstr[] = "Demo12_1: FatFS on SPI-Flash chip.\r\n";
  HAL_UART_Transmit(&huart6,startstr,sizeof(startstr),0xFFFF);

  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	//FLASH_CS

  FRESULT res=f_mount(NULL,"0:",1);
  res=f_mount(&USERFatFS, "0:", 1);	//Mount the drive
  printf("f_mount() return value= %d\r\n",res);

  // If the mount is not successful, uninstall it first and then mount it.
  if(res != FR_OK)
  {
	  BYTE workBuffer[4096];
	  res=f_mkfs("0:",FM_FAT,4096,workBuffer,4096);
  }
  else
  {
	  res=f_mount(NULL,"0:",1);							//uninstall it first
	  res=f_mount(&USERFatFS,"0:",1);				//and then mount it
  }

  if (res==FR_OK)												//Mounted successfully
	  printf("FatFS is mounted, OK.\r\n");
  else
	  printf("No file system.\r\n");

  //The menu item start line is used to clear the area when drawing the second group of menus.
  //Group 1 Menu
  printf("[1][S2]KeyUp	= Format chip.\r\n");
  printf("[2][S4]KeyLeft	= FAT disk info.\r\n");
  printf("[3][S5]KeyRight	= List all entries.\r\n");
  printf("[4][S3]KeyDown	= Next menu page.\r\n");

  KEYS waitKey;	//Key input
  while(1)
  {
	  waitKey=ScanPressedKey(KEY_WAIT_ALWAYS);  		//Waiting for the button

	  //Format Flash chips and create file system
	  if  (waitKey == KEY_UP)
	  {
		  /* The cluster size should not be less than 1 sector size.
		   * The smaller the cluster, the smaller the file space, the slower the speed,
		   * and vice versa. */
		  BYTE	workBuffer[FLASH_SECTOR_SIZE];   			//FLASH_SECTOR_SIZE=4096B, 4KB
		  DWORD clusterSize=1*FLASH_SECTOR_SIZE;		//The cluster size should be >= 1 sector

		  printf("Formatting the chip...\r\n");
		  FRESULT res=f_mkfs("0:", FM_ANY, clusterSize,  workBuffer, FLASH_SECTOR_SIZE);

		  //Create a file system. The cluster size must be greater than or equal to 1 sector.
		  //The workBuffer size should be an integer multiple of the sector size.
		  if (res ==FR_OK)
			  printf("Format OK.\r\n");
		  else
			  printf("Format fail.\r\n");
	  }
	  else if(waitKey == KEY_LEFT)
	  {
		  fatTest_GetDiskInfo();		//Get and display disk information
	  }

	  else if (waitKey == KEY_RIGHT)
	  {
		  fatTest_ScanDir("0:/");		//Scan the files and directories in the root directory
	  }
	  else if (waitKey == KEY_DOWN)
	  {
		  break;							//Jump to the next level menu
	  }
	  printf("Reselect menu item or reset.\r\n");
	  HAL_Delay(500);  				//Delay 500 to eliminate the impact of key jitter
  }

  //Group 2 Menu
  printf("[5][S2]KeyUp	= Write files.\r\n");
  printf("[6][S4]KeyLeft	= Read a TXT file.\r\n");
  printf("[7][S5]KeyRight	= Read a BIN file.\r\n");
  printf("[8][S3]KeyDown	= Get a file info.\r\n");

  HAL_Delay(500);  					//Delay 500 to eliminate the impact of key jitter

  while(2)
  {
	  //Waiting for the button
	  waitKey=ScanPressedKey(KEY_WAIT_ALWAYS);

	  if (waitKey==KEY_UP )    	//Write a file test
	  {
		  fatTest_WriteTXTFile("readme.txt",2025,6,23);
		  fatTest_WriteTXTFile("help.txt",2025,6,20);
		  fatTest_WriteBinFile("ADC500.dat",20,500);
		  fatTest_WriteBinFile("ADC1000.dat",50,1000);

		  f_mkdir("0:/SubDir1");		//Create a directory
		  f_mkdir("0:/MyDocs");		//Create a directory
	  }
	  else if (waitKey==KEY_LEFT )
		  fatTest_ReadTXTFile("readme.txt");		//Test reading text files
	  else if (waitKey==KEY_RIGHT)
		  fatTest_ReadBinFile("ADC500.dat");		//Test reading binary files
	  else if (waitKey==KEY_DOWN)
		  fatTest_GetFileInfo("ADC1000.dat");	//Test to obtain file information

	  printf("Reselect menu item or reset.\r\n");
	  HAL_Delay(500);					//Delay, eliminate the impact of key jitter
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
	  Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart6,(uint8_t*)&ch,1,0xFFFF);
	return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
