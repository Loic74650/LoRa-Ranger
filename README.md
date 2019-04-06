<h2>LoRa Ranger</h2>
<h2>Arduino compatible ultra-sound ranger LoraWan node</h2>

<h4>Brief description</h4>
	
<p>This node is based on the Adafruit Feather 32u4 board.<br />
An ultra-sound sensor measures distance to a target.<br />
Measured distance, battery voltage and some optional I/Os are then periodically published to the TTN servers over LoraWan protocol.<br />
In deep-sleep the Feather board consumption is 300uA, ie. too high for our purpose. Low power / long battery life is therefore achieved by shutting down the Feather board completely between measurements.<br />
This is done by using the Adafruit TPL5110 breakout board.<br />
A potentiometer on the TPL5110 enables the adjustment of the measurement periodicity between 0.1sec to 2 hours.<br />
Code execution takes roughly 100ms.<br />
Power consumption is approx. 20mA during that time and a negligible residual current of 50nA is drawn from the battery when the board is off.
Theoretical battery lifetime with a 500mAh LiPo is over a year for a periodicity of 60 seconds between measurements (http://www.of-things.de/battery-life-calculator.php).



<p align="center"> <img src="/pics/Schema.jpg" width="702" title="Overview"> </p> <br /><br />
<p align="center"> <img src="/pics/pic1.jpg" width="702" title="Overview"> </p> <br /><br />
<p align="center"> <img src="/pics/pic2.jpg" width="702" title="Overview"> </p> <br /><br />


<h4>Hardware</h4>
<p>
<ul>
<li><a title="https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/overview" href="https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/overview">Adafruit Feather 32u4 with LoRa Radio Module</a></li>
<li><a title="https://learn.adafruit.com/adafruit-tpl5110-power-timer-breakout/overview" href="https://learn.adafruit.com/adafruit-tpl5110-power-timer-breakout/overview">Adafruit TPL5110</a></li> 
<li><a title="http://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/" href="http://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/">Grove ultrasonic ranger</a></li>
</ul>
</p><br />

<h4>Home automation integration example (<a title="https://www.jeedom.com" href="https://www.jeedom.com">JEEDOM</a>)</h4>
<p>
<p align="center"> <img src="/pics/pic5.jpg" width="702" title="Overview"> </p> <br />
</p>
