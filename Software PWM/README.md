Author: Ben Jukus
## README_Software_PWM
# PWM
A PWM is essentially a counter and once the counter reaches a certain value it resets the counter. This is used to control an LED by having the counter act as what controls the duty cycle. To make this functionality timer modules were used in order to count up to this cap and then once the counter equaled the cap the counter was reset. 
# Logrithmic
In order to make this scale linear to the eye an array of values approximately logrithmic was created, a separate incrementor, incremented after a button press, was used to scroll through the array. 


