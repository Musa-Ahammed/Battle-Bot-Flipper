# Bill of materials

[Back to the project overview](../README.md)

The list below records the planned components. Confirm the final quantities, driver models, and current ratings against the assembled robot.

| Quantity | Part | Notes |
| ---: | --- | --- |
| 1 | Arduino Mega 2560 | Shown in the hardware images; the supplied sketch identifies an Uno. |
| 1 set | RC transmitter and receiver | At least three control channels. |
| 2 | Dual-channel Cytron brushed motor driver | Confirm drive/flipper channel allocation and ratings. |
| 2 | 24 V brushed DC traction motor | Left and right drive groups. |
| 1 | 24 V automotive wiper motor | Reversible flipper actuator. |
| 1 | 24 V battery pack | Voltage and discharge rating must suit the motors and drivers. |
| 2 listed | 24 V to 5 V buck converter | The schematic shows one; confirm the final quantity. |
| 1 | Physical emergency disconnect | Isolates battery power. |
| 1 | Main fuse and holder | Size for the wiring and expected current. |
| 3 | Branch fuse and holder | Drive, flipper, and logic branches. |
| 1 | Reverse-polarity protection stage | Confirm voltage and current rating. |
| As needed | Power wiring and connectors | Size for motor stall current and peak load. |
| 1 | Controller/interface PCB | Reference images are included; trace current capacity is unverified. |

See the [schematic](Sch-BB.jpg), [PCB layout](PCB-BB.jpg), and [testing notes](../docs/SAFETY.md). Do not assume the pictured PCB can carry the motors' full current.
