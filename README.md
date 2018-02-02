# Real Environment Supported Interactive Flight Simulator Project
Real Environment Project for Microsoft Flight Simulator X using with .NET CORE, FSUIPC API and Arduino 

The purpose of this project is to communicate with Microsoft Flight Simulator using the Arduino interface card over FSUIPC API and to make real-time, interactive flight simulator.

The communication between FUSIPC and FSX is provided by .NET CORE software architecture, and communication between electronic components and indicators is controlled via Arduino.

In general, will contain the following features (Almost %70 done)

**LCD Panel #1 - Auto Pilot Info Screen (Manual Settings with Rotary Encoder)** 

- Auto Pilot Altitude
- Auto Pilot Heading
- Auto Pilot Vertical Speed
- Auto Pilot Engage Status

**LCD Panel #2 - Radio Stack Information (Manual Setting with Rotary Encoder)** 

- Com1 and Stb1 Freq. (Pos 1)
- Com2 and Stb2 Freq. (Pos 2)
- Nav1 and Stb1 Freq. (Pos 3)
- Nav2 and Stb Freq. (Pos 4)
- DME1 and DME2 Freq. (Pos 5)
- AFD Freq (Pos 6)
- XPDRFreq (Pos 7)

**LCD Panel #3 - Flight Information Screen (with commutator selection)** 

- Current Altitude (Feet / Meters) (Pos 1)
- Current Heading (Degree) (Pos 1)
- Current Speed ​​(Knots / Mph / Mach) (Pos 1)
- Current Vertical Speed ​​(Pos 1)
- Left Motor (s) Status (Pos 2)
- Right Motor (s) Status (Pos 2)
.....

**LCD Panel #4 - Metar Information Secreen, Others** 

- Current Weather information with scrolling options
- Other text based informations wil go here...

**Real-time Flight Motion and State Model with Scale Plane***

This feature shows the realtime the plane motion and position in air with the scale model plane on your desk :)

*Also added two propeller (and fans) to simulate air flow on your face :)*

In addition, there are LED-assisted warning lights that reflect the current aircraft instrument information instantly and visually. These warnings in general; Flap states, Landing gear states (each one individually), deceleration flap state, important warnings and motor conditions.

Control of all other instruments can be controlled by various switches and knobs. All selections and settings are visually displayed with extra panels and LEDs.

(To be continued)
