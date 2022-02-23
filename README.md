<a id="library"></a>

# gbj\_appdisplay\_tm7
This is an application library, which is used usually as a project library for particular PlatformIO project. It encapsulates the functionality of a `TM1637 digital 7-segment display module`. The encapsulation provides following advantages:

* Functionality is hidden from the main sketch.
* The library follows the principle `separation of concerns`.
* The library is reusable for various projects without need to code 7-segment display modules management.
* Update in library is valid for all involved projects.
* It specifies (inherits from) the parent application library `gbj_appcore`.
* It utilizes funcionality and error handling from the parent class.


## Fundamental functionality
* The functionality is represented by methods managing various modes of printing text on the display module with 7-segment tubes controlled by TM1637 driver.
* For included font definition, the displayed text can contain only `digits, decimal point, space, and minus sign`.
* The library utilizes internal timer for blinking and breathing display effects.


<a id="internals"></a>

## Internal parameters
Internal parameters are hard-coded in the library usually as enumerations and have neither setters nor getters associated.

* **Time period for blinking and breathing** (500 ms): This parameter defines blinking or breathing display with frequency 1 blink per second (1 Hz).


<a id="dependency"></a>

## Dependency
* **gbj\_appcore**: Parent library for all application libraries loaded from the file `gbj_appcore.h`.
* **gbj\_timer**: Library for executing internal timer within an instance object loaded from the file `gbj_timer.h`.
* **gbj\_serial\_debug**: Auxilliary library for debug serial output loaded from the file `gbj_serial_debug.h`. It enables to exclude serial outputs from final compilation.
* **gbj\_tm1637**: Library for processing `TM1637` display module loaded from the file `gbj_tm1637.h`.
* **font7seg\_decnums.h**: Font definition for display with decimal digits, space, and minus glyph.

#### Arduino platform
* **Arduino.h**: Main include file for the Arduino SDK.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.

#### Espressif platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.


<a id="constants"></a>

## Constants
* **VERSION**: Name and semantic version of the library.

Other constants, enumerations, result codes, and error codes are inherited from the parent library.


<a id="interface"></a>

## Interface
* [gbj_appdisplay_tm7()](#gbj_appdisplay_tm7)
* [begin()](#begin)
* [run()](#run)
* [blinkStart()](#blink)
* [blinkStop()](#blink)
* [breathStart()](#breath)
* [breathStop()](#breath)
* [printText()](#printText)
* [printTextBlink()](#printTextEffect)
* [printTextBreath()](#printTextEffect)
* [printError()](#printError)
* [printInit()](#printInit)
* [printData()](#printData)

### Setters
* [setData()](#setData)
* [setDataPrintOn()](#setDataPrint)
* [setDataPrintOff()](#setDataPrint)

### Getters
* [getDisplayPtr()](#getDisplayPtr)


<a id="gbj_appdisplay_tm7"></a>

## gbj\_appdisplay\_tm7()

#### Description
Constructor creates the class instance object and initiates internal resources.

* It inputs operational parameters for hardware configuration.
* It creates an internal timer for display blinking (toggling display on/off) and breathing (toggling brightness max/min).
* It creates an internal object for controlling TM1637 display.

#### Syntax
    gbj_appdisplay_tm7(byte pinClk, byte pinDio, byte digits)

#### Parameters
* **pinClk**: Number of GPIO pin of the microcontroller managing the serial clock line of the display.
  * *Valid values*: positive integers 0 ~ 255
  * *Default value*: none


* **pinDio**: Number of GPIO pin of the microcontroller managing the serial data line of the display.
  * *Valid values*: positive integers 0 ~ 255
  * *Default value*: none


* **digits**: Number of 7-segment digits of the TM1637 to be utilized. With number less than physical number of tubes it is possible to reduce display region for long displays.
  * *Valid values*: positive integers 0 ~ 6
  * *Default value*: 4

#### Returns
Object performing display module management.

[Back to interface](#interface)


<a id="begin"></a>

## begin()

#### Description
The initialization method of the instance object, which should be called in the setup section of a sketch.
- The method activates an object for TM1637 display.
- It loads font from used include file.

#### Syntax
    void begin()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="run"></a>

## run()

#### Description
The execution method, which should be called frequently, usually in the loop function of a sketch.
* The method controls blinking and/or breathing of the display, if it is activated.

#### Syntax
	void run()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="blink"></a>

## blinkStart(), blinkStop()

#### Description
The corresponding method activates or deactivates blinking of the display.
* Blinking of the display is realized by periodically turning it on and off.
* At blinking activation the method sets the maximal brightness of the display.
* At blinking deactivation the method sets the minimal brightness of the display.
* Activation of the blinking is realized by resuming internal timer, if it is not running so far due to already activated breathing of the display.
* At deactivation of the blinking the internal timer is halted, if the breathing is not activated already.
* The display is able to blink and breath at the same time.

#### Syntax
	void blinkStart()
	void blinkStop()

#### Parameters
None

#### Returns
None

#### See also
[breathStart()](#breath)

[breathStop()](#breath)

[Back to interface](#interface)


<a id="breath"></a>

## breathStart(), breathStop()

#### Description
The corresponding method activates or deactivates breathing of the display.
* Breathing of the display is realized by periodically setting maximal and minimal brightness (contrast).
* At breathing activation the method sets the maximal brightness of the display.
* At breathing deactivation the method sets the minimal brightness of the display.
* Activation of the breathing is realized by resuming internal timer, if it is not running so far due to already activated blinking of the display.
* At deactivation of the breathing the internal timer is halted, if the blinking is not activated already.
* The display is able to blink and breath at the same time.

#### Syntax
	void breathStart()
	void breathStop()

#### Parameters
None

#### Returns
None

#### See also
[blinkStart()](#blink)

[blinkStop()](#blink)

[Back to interface](#interface)


<a id="printText"></a>

## printText()

#### Description
The method shows the provided text starting from the provided digit on the display.
* According to loaded (included) font the display ignores glyphs not defined in the font. Usually it accepts only digits, decimal point, space, and minus sign.

#### Syntax
	void printText(String msg, byte digit)

#### Parameters
* **msg**: Text to shown on the display.
  * *Valid values*: digits, decimal point, space, minus sign
  * *Default value*: none


* **digit**: Number of digit counting from zero of the display where the showing of the text should start.
  * *Valid values*: positive integers 0 ~ [digits - 1](#gbj_appdisplay_tm7)
  * *Default value*: 0

#### Returns
None

[Back to interface](#interface)


<a id="printTextEffect"></a>

## printTextBlink(), printTextBreath()

#### Description
The corresponding method activates particular effect and shows the provided text starting from the provided digit on the display using the method [printText()](#printText).

#### Syntax
	void printTextBlink(String msg, byte digit)
	void printTextBreath(String msg, byte digit)

#### Parameters
Same as for [printText()](#printText).

#### Returns
None

#### See also
[printText()](#printText)

[Back to interface](#interface)


<a id="printError"></a>

## printError()

#### Description
The method shows the provided maximal error code surrounded with dashes on the display using the method [printText()](#printText).
* The maximal displayed error code depends on number of digits declared in [constructor](#gbj_appdisplay_tm7). By default it is `-00-` to `-99-`.
* The method does not change blinking or breathing mode of the display. It should be set before error code displaying.

#### Syntax
	void printError(unsigned int err)

#### Parameters
* **err**: Numeric error code to be displayed.
  * *Valid values*: positive integer 0 ~ 9999
  * *Default value*: none

#### Returns
None

#### See also
[printText()](#printText)

[Back to interface](#interface)


<a id="printInit"></a>

## printInit()

#### Description
The method shows the textual pattern composed just of dashes using the method [printText()](#printText), which acts as a initialization mark.
* The displayed depends on number of digits declared in [constructor](#gbj_appdisplay_tm7). By default it is `----`.
* The method does not change blinking or breathing mode of the display. It should be set before error code displaying.

#### Syntax
	void printInit()

#### Parameters
None

#### Returns
None

#### See also
[printText()](#printText)

[Back to interface](#interface)


<a id="printData"></a>

## printData()

#### Description
The method shows a real number injected to the library instance object if it is enabled.
* The displayed number is rounded to 1 decimal place.

#### Syntax
	void printData()

#### Parameters
None

#### Returns
None

#### See also
[printText()](#printText)

[setData()](#setData)

[Back to interface](#interface)


<a id="setData"></a>

## setData()

#### Description
The method receives real number from external sources, e.g., from a call back function even if the display is being used for showing other text at this time.

#### Syntax
	void setData(float data)

#### Parameters
* **data**: Real number to be displayed.
  * *Valid values*: floating number
  * *Default value*: none


#### Returns
None

#### See also
[printText()](#printText)

[printData()](#printData)

[Back to interface](#interface)


<a id="setDataPrint"></a>

## setDataPrintOn(), setDataPrintOff()

#### Description
The corresponding method enables or disables (suspends) the displaying the received data by the method [setData()](#setData).
* The receiving of data is not affected, just the displaying it.
* At enabling the displaying data the method deactivates eventual blinking or breathing mode of the display.

#### Syntax
	void setDataPrintOn()
  void setDataPrintOff()

#### Parameters
None

#### Returns
None

#### See also
[printData()](#printData)

[setData()](#setData)

[Back to interface](#interface)


<a id="getDisplayPtr"></a>

## getDisplayPtr()

#### Description
The method returns the pointer to the internal object controlling the TM1637 display itself. It allows to utilize entire interface of the corresponding library `gbj_tm1637` without wrapping or mirroring it.

#### Syntax
	gbj_tm1637 *getDisplayPtr()

#### Parameters
None

#### Returns
Pointer to the control object of the TM1637 display hardware.

#### Example
```cpp
gbj_appdisplay_tm7 display = gbj_appdisplay_tm7(...);
void setup()
{
  display.begin();
  display.getDisplayPtr()->setContrast(3);
}
```

[Back to interface](#interface)
