# Xbox Controller Driver for macOS

## Status
Work-in-progress, currently it produces a Fake360Test executable that enables the controller to report its input and displays it to console for debugging purposes.
The executable is also a proof of concept, it creates a working virtual device that relays the actions from the real device.

## About
Fake360Driver is an another Xbox 360 controller driver which operates in userland similar to xboxdrv on linux, allowing me to use it on OSX without the requirement to codesign it.
This project was started after my [issue](https://github.com/360Controller/360Controller/issues/384) with the 360Controller driver was closed down without a fix and without finding any working alternative.

The Xbox360 Controller I own is a clone bought from Taobao labeled PC188 and is advertised as PC/XBOX360 compatible.

System Information report:
```
Controller:

  ID prodotto:	0x028e
  ID fornitore:	0x045e  (Microsoft Corporation)
  Versione:	1.10
  Velocità:	Fino a 12 Mb/sec
  Produttore:	SHANWAN
  ID posizione:	0x14200000 / 13
  Corrente disponibile (mA):	1000
  Corrente necessaria (mA):	500
  Operazione extra della corrente (mA):	0
```

I'm using this also as an opportunity to learn CMake.

## Requirements
* foohid ([download link](https://github.com/unbit/foohid/releases/download/0.2.1/foohid-0.2.1.dmg))

## Usage
```
cd build/
cmake -G Xcode .. # On OSX - open the produced project and build it
```