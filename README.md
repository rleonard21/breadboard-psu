# Overview
I was bored one weekend and decided that I needed an easy way to generate a 3.3v rail on my breadboard without a real power supply.
So I designed this simple circuit to perform just that. The device is powered from USB and has two independent output channels, one for 
each rail on the breadboard. A button per channel makes for easy toggling of the power for a rail. The device is also equipped with
current and voltage sensors for detecting over-current or under-voltage conditions. The breadboard power supply doesn't nearly
match the performance of a true power supply, but it's simple and easy to work with and didn't cost nearly as much. 

# Specs
* Two individual output channels, one for each rail
* User selectable output voltage from 1.8v, 3.3v, or 5v
* Channel enable/disable per channel
* Over-current protection (device disables output on over-current condition)
* Under-voltage indication

# Images
Here's the circuit all assembled and ready to go:
![The assembled circuit.](https://imgur.com/kgjNZM0.jpg)

The circuit fits right into the power rails of a standard breadboard.
![The circuit fits right into a breadboard.](https://imgur.com/ptQ6XpQ.jpg)
