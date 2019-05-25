# Ｃｏｍｐｌｅｘｉｔｙ Rooms Client Code

## Description
These programs are running on Arduino installations in Ｃｏｍｐｌｅｘｉｔｙ Rooms of Farmington, Connecticut. Each is used to monitor and respond to the progress of escape room players as they complete various puzzles. Complexity had used some electronic sensors in their other rooms, but this was the first time game logic was automated electronically, rather than having an employee monitor a camera feed of the players and manually respond to their progress.

## Considerations
The room these puzzles are in is dimly lit, which makes it hard for an employee to see them through a camera feed. There are also many electronic devices powered by relays that trigger when these puzzles are solved, which are easy to use with microcontrollers. Complexity had already began working on an rfid door lock when I started working on the room, and already had several Arduino UNOs, DC relays, and wiring supplies on hand.

## Design
Every puzzle needs to have at least two states, including unsolved and solved. The transition from unsolved to solved is what players try to figure out during their session, the transition back from solved to unsolved is what employees use to reset the room between sessions. Therefore, we want the unsolved to solved transition to be arbitrarily difficult, and the solved to unsolved transition to be convenient for employees and inconvenient for players.

### Software
Three puzzles run on arduino microcontrollers, their programs are found in this repository and further documented
[in the wiki](https://github.com/mayhd3/Complexity/wiki/Arduino-Programs).
One puzzle runs on five
[escapekeeper juniors](https://www.frightideas.com/escapekeeper-jr.html),
in the configuration described
[in the wiki.](https://github.com/mayhd3/Complexity/wiki/EscapeKeeper-Setup)

### Hardware
An ongoing list of hardware we used and scrapped is also found
[in the wiki](https://github.com/mayhd3/Complexity/wiki/Hardware-Performance).
The list includes parts we did not ultimately use.
