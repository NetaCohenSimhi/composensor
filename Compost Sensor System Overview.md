### **Temperature Monitoring** ###
Temperature readings guide us in determining the phase of composting:
- Phase 1: 45°C
- Phase 2: 50-60°C
- Phase 3: Above 70°C

**Code info:** The [WiFi-Connected Temperature Sensor](https://github.com/NetaCohenSimhi/composensor/blob/main/WiFi-Connected%20Temperature%20Sensor%20.ino) code reads temperature data from a DS18B20 sensor and sends the data to ThingSpeak for remote monitoring.
[full code to copy](https://github.com/NetaCohenSimhi/composensor/blob/main/WiFi-Connected%20Temperature%20Sensor%20.ino) )

![Alt Text](https://y.yarn.co/31282a9d-bf87-426e-8195-ed8bf8611823_text.gif)


### **Oxygen Level Analysis**

Oxygen levels between 5% to 15% are ideal for bacterial development. If levels fall below 5%, an automated motor is activated to aerate the compost, allowing for even oxygen distribution.

*Disclaimer: We did not have an oxygen sensor, so we wrote a function that generates random oxygen levels between 0 and 20 percent.*

**Code info:** The [WiFi-Connected Oxygen Level Monitor and Motor Controller](https://github.com/NetaCohenSimhi/composensor/blob/main/WiFi-Connected%20Oxygen%20Level%20Monitor%20and%20Motor%20Controller.ino) code generates random oxygen levels and controls a motor based on them.
[full code to copy](https://github.com/NetaCohenSimhi/composensor/blob/main/Oxygen%20Level%20Motor%20Control%20Code.ino)

![Alt Text](https://y.yarn.co/f7ab8765-36cd-4591-a7ec-dde4d0311bc1_text.gif)


### **Humidity Control** ###
Efficient soil moisture management is crucial:
- <30% moisture inhibits bacterial activity, particularly problematic during hot, dry periods.
- >65% moisture leads to sluggish decomposition, foul odors, and nutrient leaching.
- For moisture levels <30%, a water tap is activated to boost moisture content.

**Code info:** The [Soil Moisture Auto-Irrigation Controller](https://github.com/NetaCohenSimhi/composensor/blob/main/Soil%20Moisture%20Auto-Irrigation%20Controller.ino) code controls an irrigation system based on soil moisture readings.
[full code to copy](https://github.com/NetaCohenSimhi/composensor/blob/main/Soil%20Moisture%20Auto-Irrigation%20Controller.ino) )

![Alt Text](https://media1.tenor.com/m/tloalQrDlOIAAAAC/bad-hair-day-its-the-humidity.gif)


[Click to get back to the first page](https://github.com/NetaCohenSimhi/composensor/blob/main/README.md)



