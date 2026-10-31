# ESP32 Version of Miro Samek's Modern Embedded Programming Course for State Machines

This is an ESP32 + FreeRTOS version of Miro Samek's famous Modern Embedded Programming Course: State Machines.
After having a difficult time with newer versions of Keil uVision locking my Tiva C Series LaunchPad, I decided to try to replicate the material that Miro Samek goes over for State Machines with my ESP32 + FreeRTOS in ESP-IDF. This captures the fundamentals of the State Machine material that Miro covers, while also giving another persepective on how to implement these development patterns. I also feel that I gain more confidence having to port Miro's code from what is shown in the courses as weel as gain a better understanding of what is actually happening (and not just "parroting" what he is doing).

[Miro Samek's Modern Embedded Programming Course: State Machines](https://youtube.com/playlist?list=PLPW8O6W-1chxym7TgIPV9k5E8YJtSBToI&si=RVeopEgoiqhwVXN7)

#### Note:
Please ensure the you have enabled FreeRTOS's ```vApplicationTickHook``` using the config setting: ```configUSE_TICK_HOOK```

### This is the code for the end of Lesson 39