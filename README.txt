EMERGECNY STOP TRANSMITTER README

VERSION 1 - JUNE 21 2019

CONTENTS:
  1. 3D Printing Case
  2. Assembly
  3. Programming
  4. Configuring
  5. Operation



1. 3D Printing Case

The case of the E-Stop transmitter is comprised of two parts: an upper receiver and a lower receiver.  Each receiver houses several components, and the two parts mate together to form the body of the transmitter.

The design files for these parts have been provided in this package:
  - Lower Receiver.stp
  - Upper Receiver.stp
  
These parts are intended to be 3D printed.  The original design allows for printing on a standard desktop FDM 3D printer with a single extruder.  Other printers may be used at your own discretion.

It is recommended that the parts be printed in ABS plastic, however PLA or another form of plastic may be used as well.

For printing on a single-extruder printer without dissolvable filament, both receivers must be printed with the interior (hollow) face mated to the bed of the printer.  Supports must be used.  We recommend using Simplify 3D as a slicer because it has superior support structure generation.

Curling may be an issue for ABS prints due to the low surface area mating the part to the bed.  A raft is recommended for all plastics.  For ABS, a heated build plate and enclosed print volume will be necessary.

After printing, ensure that all dissolvable or supporting material is fully removed, or certain components will not fit (especially the rechargeable battery).



2.  Assembly

Tools Needed: Soldering Iron + Solder, Wire Cutter/Stripper, 2mm and 2.5mm Hex Drivers, Crescent Wrench

The schematic has been provided in this package:
  - Schematic.jpg

The assembly for the transmitter is relatively complicated due to the dense population of components inside of the upper and lower receivers.  It is recommended that you first place the components into the receivers, follow the schematic, and cut wires to the appropriate lengths.  Keep in mind spacing required for putting the two receiver halves together.

NOTE: Before soldering components, if you only have one XBee USB Adapter board, perform the configuration step for the XBee first (Section 4 of this README).  This allows you to configure the XBee using the USB adapter board before soldering and integrating it into the lower receiver.

Once the wire lengths are measured and cut, remove the components and begin soldering wires and resistors into place using the included schematic.  Be careful that nearby resistors leads are not touching.  It is VERY IMPORTANT that you follow the schematic exactly.  If done incorrectly, you may damage one or more of the components.  You may also cause the Lithium Ion rechargeable battery to catch fire.

The toggle switch must have its two bottom pins/pads bent outwards at a 90deg angle in order to fit into the lower receiver.

The RC rectangular header is needed to physically separate the two receiver halves since they are connected by a shared wiring harness.  The best location for this connector is directly above the XBee radio.  Ensure that the connector attached to the XBee and the Battery uses FEMALE PINS.

Do not attach the locking spacer and nut onto the XBee RP-SMA until after the upper and lower receivers are attached together.  Once the locking spacer and nut are attached, you may attach the duck antenna.  You may use a crescent wrench or other similar wrench for tightening the nut on the XBee RP-SMA connector.

When attaching the two receivers with the flathead screws, you have the option of either tapping the holes with an M2.5 metric tap, or screwing directly into the hole without tapping.  Tapping the hole ahead of time will make assembly easier, but increases the risk of stripping the plastic.  The screws can be self-tapped with a little more torque, but again be careful to not strip the plastic.  You may have to compress the two receivers together to align all of the holes.



3.  Programming

This package includes and Arduino sketch in the form of an INO file.  This file is ready to be uploaded to the trinket via USB.  Adafruit has provided a step-by-step guide for configuring a PC and the Arduino IDE to upload sketches to the Trinket.  The link can be found below.  Please follow these directions VERY CAREFULLY.  You must ensure that you choose the 3V version of the board, not the 5V version.  You must also make sure to choose the USBtinyISP as the programmer.  Failure to take these vital steps may brick the Trinket.
https://learn.adafruit.com/introducing-pro-trinket/downloads

Once your PC and Arduino IDE is configured to upload sketches to the Trinket, you may upload the INO file/sketch provided in this package.



4.  Configuring

Next, you must configure the XBee in the transmitter.  This can be accomplished using Digi's XCTU utility and the XBee configuration file provided in this package.  Plug your XBee into a PC running XCTU through the USB XBee Adapter baord.  Next, use the XCTU configuration uploading tool to upload the configuration to the XBee that will be installed in the transmitter.  Once you upload the configuration to the XBee through XCTU, ensure that the encryption key is set to match your team's supplied private encryption key.



5.  Operation

Charging: The USB Micro B port located at the bottom of the transmitter handle is used to charge the internal battery.  Connect this to a 5V, 2A USB power supply (phone chargers are fine).  You will see a yellow/orange light turn on inside of the transmitter, which will be visible through the 3D printed plastic if you used orange ABS or PLA.  This light indicates that it is charging.  The charger circuit will automatically turn off when the battery is fully charged.

Usage: When you want to trigger an emergency stop, simply open the toggle switch guard and flip the switch to the ON position.  You will see the light on the toggle switch turn on.  This will immediately start broadcasting emergency stop messages, and will continue to do so until you switch the toggle switch back into the OFF position.