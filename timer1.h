/*
 * timer1.h
 *
 *  Created on: 15 июл. 2023 г.
 *      Author: User
 */

#ifndef TIMER1_H_
#define TIMER1_H_

static LARGE_INTEGER frequency;
static LARGE_INTEGER startCount, endCount;
static double elapsedTime;

void
timer1_init (void)
{
  // Получаем частоту производительного счетчика
  QueryPerformanceFrequency (&frequency);
}

void
timer1_t1 (void)
{
  // Запускаем счетчик времени
  QueryPerformanceCounter (&startCount);
}

double
timer1_t2_res_us (void)
{
  // Останавливаем счетчик времени
  QueryPerformanceCounter (&endCount);
  // Вычисляем затраченное время в секундах
  elapsedTime = (double) (endCount.QuadPart - startCount.QuadPart)
      / frequency.QuadPart * 1.0e6;
  //printf("Прошло времени: %.6f микросекунд\n", elapsedTime);
  return elapsedTime;
}

#endif /* TIMER1_H_ */
