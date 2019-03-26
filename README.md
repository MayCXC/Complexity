# Complexity Rooms Client Code

## Description
These programs are running on Arduino installations in Complexity Rooms of Farmington, Connecticut. Each is used to monitor and respond to the progress of escape room players as they comlpete various puzzles. Complexity had used some electronic sensors in their other rooms, but this was the first time game logic was automated electronically, rather than having an employee monitor a camera feed of the players and respond to their progress manually.

## Considerations
The room these puzzles are in is dimly lit, which makes it hard for an employee to see them through a camera feed. There are also many electronic devices powered by relays that trigger when these puzzles are solved, which are easy to use with microcontrollers. Complexity had already began working on an rfid door lock when I started working on the room, and already had serveral Arduino UNOs, DC relays, and wiring supplies.

## Design
Every puzzle needs to have at least two states, including unsolved and solved. The transition from unsolved to solved is what players try to figure out during their session, the transition back from solved to unsolved is what employees use to reset the room between sessions. Therefore, we want the unsolved to solved transition to be arbitrarily difficult, and the solved to unsolved transition to be simple for employees and hard for players.

## Hardware Performance
### SPI RFID Readers, High Frequency RFID Tags
#### Very Unreliable
The door lock was originally implemented with four MiFare rfid card readers. Each worked well seperately, but gave very unreliable readings when used together. Our initial researh turned up examples of multiple readers connected to one arduino using jumper wires, but when we used longer cables their reliability dramatically decreased. Our tests showed at short distances, the readers could reliably share SCK, RST, MISO, MOSI, VCC, and GND connections, and use a seperate SDA connections. At long distances, we could reliably use a single reader. Sadly, we could not do both.

### UART RFID Readers, Low Frequency RFID Tags
#### Unreliable
After testing the shortcomings of the first door lock and further research, the door lock was refit with four seeed uart rfid readers. These readers had support for external antennas, which was a better design for the structure of the frames. They also only required four connections per reader, two shared and two seperate. Arduino UNOs only have one hardware serial input, so the readers connected to four digital inputs, and the software serial library was used to read from each one after the next. Unfortunately, these readers only read a tag once, so any noise or interruption in the serial connection completely prevents a tag from being read. While all four readers worked in this configuration, they were not reliable enough to use in the final puzzle.

Both types of RFID sensors sent power and data through Cat5e cables at TTL voltages. This type of connection is only meant to span short distances; further research showed that to reliably extend it, we would have to use two RS232 shifters per serial connection, eight shifters in total. Each shifter would also have to be powered with eight DC power connections in addition to our four serial conections, which was too demanding. Even using rfid tags for the puzzle now seemed too demanding; one byte of data at sixty baud was more than enough.

### Hall Effect Sensors, Neodinium Magnets
#### Very Reliable

### Arduino UNO
#### Reliable

### Cat5e Cables, RJ45 Connectors
#### Very Reliable
Longer connections in the door lock and fireplace are cut and crimped carry both 5v power and TTL signals.
VCC, GND, TX and RX in a single cable.

### Breadboard, Power Supplies, Dupont Jumper Wires
#### Reliable
Although they short circuits, two read ten volts rather than the expected five.

### RGB LEDs
#### Reliable
Soldering jumper wires to these is tricky.
