# Complexity Rooms Client Code

## Description
These programs are running on Arduino installations in Complexity Rooms of Farmington, Connecticut. Each is used to monitor and respond to the progress of escape room players as they complete various puzzles. Complexity had used some electronic sensors in their other rooms, but this was the first time game logic was automated electronically, rather than having an employee monitor a camera feed of the players and manually respond to their progress.

## Considerations
The room these puzzles are in is dimly lit, which makes it hard for an employee to see them through a camera feed. There are also many electronic devices powered by relays that trigger when these puzzles are solved, which are easy to use with microcontrollers. Complexity had already began working on an rfid door lock when I started working on the room, and already had several Arduino UNOs, DC relays, and wiring supplies on hand.

## Design
Every puzzle needs to have at least two states, including unsolved and solved. The transition from unsolved to solved is what players try to figure out during their session, the transition back from solved to unsolved is what employees use to reset the room between sessions. Therefore, we want the unsolved to solved transition to be arbitrarily difficult, and the solved to unsolved transition to be simple for employees and hard for players.

# Arduino Programs

## hall-effect-light
This program turns on a relay when eight hall effect sensors activate. In the room, it is used to turn on the fireplace when each dowel is in the right hole and facing forwards.

### Win Condition
All eight dowels are in place.

### Reset Condition
All eight dowels are removed.

## hall-effect-lock
This program uses the same logic as hall-effect-light to open a hidden door lock when all four pictures hang on certain pegs.

### Win Condition
Pictures hang in the correct position. By number of penguins:
```
1 4
3 2
```

### Reset Condition
All four pictures are removed.

## led-piezo-light-lock
This program lights six NeoPixel LEDs inside wax candles when a player blows on a piezo sensor positioned behind their wick. When all six candles are blown on in the right order, it activates a relay. In the room, this relay opens a secret door. Blowing the candles on in the wrong order or taking too long to finish resets the puzzle. For safety, the relay is not reactivated on until the next power cycle.

### Win Condition
Candles are blown on in order. By order from 0 to 5 left to right:
`2 4 1 0 3 5`

### Reset Condition
Players take too long to blow out the next candle.

## quad-spi-rfid-lock
This program opens a maglock when fish props are hung on the correct hooks. This is logically equivalent to the first version of what is now the **hall-effect-lock** puzzle, but it uses shorter wires and an undocumented fix to make the readers work more reliably.

### Win Condition
Fish hang in the correct position.

### Reset Condition
All four fish are removed.

## quad-uart-rfid-lock
This program tries to read four rfid tag IDs and check if they are in a certain order. Reading the rfid tags never worked reliably enough to use these in a puzzle.

## Hardware Performance
### SPI RFID Readers, High Frequency RFID Tags
#### Very Unreliable, Workaround Possible
The door lock was originally implemented with four MiFare rfid card readers. Each worked well separately, but gave very unreliable readings when used together. Our initial research turned up examples of multiple readers connected to one Arduino using jumper wires, but when we used longer cables, their reliability dramatically decreased. Our tests showed at short distances, the readers could reliably share SCK, RST, MISO, MOSI, VCC, and GND connections, and use a separate SDA connections. At long distances, we could reliably use a single reader. Sadly, we could not do both. An undocumented fix allowed us to use these reliably enough at shorter distances for the fish puzzle.

### UART RFID Readers, Low Frequency RFID Tags
#### Unreliable
After testing the shortcomings of the first door lock and further research, the door lock was refit with four seeed uart rfid readers. These readers had support for external antennas, which was a better design for the structure of the frames. They also only required four connections per reader, two shared and two separate. Arduino UNOs only have one hardware serial input, so the readers connected to four digital inputs, and the software serial library was used to read from each one after the next. Unfortunately, these readers only read a tag once, so any noise or interruption in the serial connection completely prevents a tag from being read. While all four readers worked in this configuration, they were not reliable enough to use in the final puzzle.

___

Both types of RFID sensors sent power and data through cat5e cables at TTL voltages. This type of connection is only meant to span short distances; further research revealed that to extend it reliably, we would have to use two rs232 shifters per serial connection, so eight shifters in total. The shifters would also have to be powered with eight separate DC power connections, in addition to the four serial connections, which was too demanding. Even using rfid tags for the puzzle now seemed too demanding; one byte of data at sixty baud was more than enough for what we needed, and far less than the readers provided.

___

### Hall Effect Sensors, Neodymium Magnets
#### Very Reliable
This type of sensor simply activates when a magnet is nearby, and works reliably at TTL voltages across our required distances. They do not detect unique magnets, so they are positioned differently to match different elements of the puzzles. Powering eight sensors was no problem for the fireplace, so we chose to use them to replace the four rfid card readers in the painting puzzle, and got much better results. Some sides of the cube magnets only activate the sensors at certain angles, so the least finicky side was positioned to be facing the sensors.

### Arduino UNO
#### Reliable
This is the microcontroller of choice used in all of the puzzles. Its most useful features are the pulldown resistors built into the digital inputs, and the abundance of code examples available online. Most of the programs could be replaced with on board logic gates and latches, but Arduinos are reusable, and being able to reprogram them when puzzles change or I am at school proved very valuable.

### Cat5e Cables, RJ45 Connectors
#### Very Reliable
Longer connections in the door lock and the fireplace use cut and crimped cat5e cables to carry both 5v power and TTL signals. They work very well, but surprisingly we found no hardware capable of reliably carrying VCC, GND, TX and RX to a serial device with a single cable, which was our main use case for them.

### Power Supplies, Breadboard, DuPont Jumper Wires
#### Reliable
The power supplies provide a reliable 5v output from a 12v dc power connection, which here is coming from an adapter plugged into the wall. Although they are advertised as protecting against short circuits, the two supplies I shorted now read ten volts on a multimeter, rather than the expected five. To avoid this, and to keep up with the constant structural changes to the puzzles, the components were connected with breadboards and dupont wires. (The less stripping and soldering, the better.)

### RGB LEDs
#### Reliable
Soldering jumper wires to these is tricky, but they are very bright and colorful. The lights were first embedded into wax candles with hot glue, but they heated up enough to melt the wax through the glue, so they are now hanging freely inside the candles. I first tried programming them with the FastLED library I was familiar with, but the vendor Adafruit NeoPixel library worked better for multiple outputs.

### Wind Sensor rev C
#### Unreliable
This anemometer worked well for detecting when a player blew on them, but while I was between visits every single one of them failed. Once again we decided that they were excessive; they are meant to produce accurate wind speed readings, and we just need to know the order that the sensors are blown on, not how hard they are blown on.

### Piezo Sensors
#### Very reliable
Simple piezo sensors worked far better than the wind sensors, their analog reading reliably drops to zero when they are blown on. I expected them to also pick up noise in the room, as they are normally used as acoustic sensors, but no amount of clapping and stomping had the same effect as blowing on them. Surprisingly, researching this use case online found nothing. The sensor readings slowly rise while they are powered, but they seem to top out eventually, and this has had no consequences yet.

### Magnetic Door Locks, DC Relays
#### Reliable
The alignment of the magnet attachment is very finicky, but once it is in place, it holds very strong against a straight pull away from the lock. The relays reset with the Arduinos so that the room can be easily reset between sessions.
