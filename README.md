# LAB4: A Cooperative Scheduler
- Main program has 5 tasks running periodically in 0.5s, 1s, 1.5s, 2s, 2.5s.
- Specifically setup and demonstrate:
  - A regular 10ms timer tick
  - Before entering the main loop, setup a few calls to SCH_Add_Task. Make sure the delay used     is long enough such that loop is entered before these wake up.
 - The following designs are considered unsatisfactory:
  - O(n) searches in the SCH_Update function.
  - Interrupt frequencies greater than 10Hz, if your timer ticks regularly.
