/* Core/Src/TaskBlinker.c */
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#define BLUE_LED_PORT GPIOB
#define BLUE_LED_PIN  GPIO_PIN_7  // Onboard Blue LD2 for STM32F767 Nucleo

/**
 * @brief FreeRTOS task that toggles the Nucleo board's User Blue LED (PB7).
 * @param argument Unused task parameter wrapper.
 */
void TaskBlinker(void *argument)
{
  /* USER CODE BEGIN 5 */
  #define MAX_PRESSES     15
  #define MAX_STEPS       29  // Exactly 15 notes + 14 rests between them
  #define BUTTON_PRESSED  (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
  #define USER_LED_ON     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
  #define USER_LED_OFF    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)

  struct RecordedStep {
    uint8_t is_note;
    uint32_t duration_ms;
  };

  struct RecordedStep pattern[MAX_STEPS];
  size_t total_recorded_steps = 0;

  // Wait for the very first physical button press while blinking the heartbeat
  while (!BUTTON_PRESSED) {
    if ((osKernelGetSysTimerCount() / 150) % 2 == 0) {
      USER_LED_ON;
    } else {
      USER_LED_OFF;
    }
    osDelay(10);
  }
  USER_LED_OFF;
  osDelay(100); // Debounce guard

  // Linear recording loop for exactly 15 sequential presses
  for (size_t press = 0; press < MAX_PRESSES; press++) {
    uint32_t duration = 0;

    // Record the HOLD time (Note)
    USER_LED_ON;
    while (BUTTON_PRESSED) {
      osDelay(10);
      duration += 10;
    }
    USER_LED_OFF;

    // Save the Note Step safely into the array
    pattern[total_recorded_steps].is_note = 1;
    pattern[total_recorded_steps].duration_ms = duration;
    total_recorded_steps++;

    // Record the RELEASE time (Rest) - Skip this only on the final 15th press
    if (press < (MAX_PRESSES - 1)) {
      duration = 0;
      // Loop runs continuously while the button is NOT pressed
      while (!BUTTON_PRESSED) {
        osDelay(10);
        duration += 10;
      }

      // Save the Rest Step safely into the array
      pattern[total_recorded_steps].is_note = 0;
      pattern[total_recorded_steps].duration_ms = duration;
      total_recorded_steps++;
    }
  }

  // Flash 3 quick times to show the 15th press locked in successfully
  for(int x = 0; x < 3; x++) {
    USER_LED_ON; osDelay(100); USER_LED_OFF; osDelay(100);
  }
  osDelay(1000);

  // Playback the saved pattern
  for (;;) {
    for (size_t i = 0; i < total_recorded_steps; i++) {
      uint32_t step_time = pattern[i].duration_ms;

      if (pattern[i].is_note) {
        // Enforce the 50ms pulse limit rule on your custom recorded notes
        if (step_time >= 50) {
          USER_LED_ON;
          osDelay(50);
          USER_LED_OFF;
          osDelay(step_time - 50);
        } else {
          USER_LED_ON;
          osDelay(step_time);
          USER_LED_OFF;
        }
      } else {
        // Maintain silence for the recorded rest interval
        USER_LED_OFF;
        osDelay(step_time);
      }
    }

    // 2 second distinct phrase boundary break before looping your 15 notes again
    osDelay(2000);
  }
  /* USER CODE END 5 */
}


