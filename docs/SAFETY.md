# Testing notes

[Back to the project overview](../README.md)

- Start with motor power disconnected and verify the controller, pin assignments, and RC channels.
- For drive tests, raise the wheels and disconnect the flipper. For flipper tests, keep its full travel area clear and use a controlled test setup.
- Keep a physical battery disconnect accessible. Software shutdown does not isolate power.
- Test receiver behavior when the transmitter is switched off. A receiver that holds its last valid pulse may keep the motors commanded.
- The sketch has no neutral-start check, flipper limit-switch handling, or run-time limit. Valid commands can move the robot after its startup delay.
- Size drivers, wiring, connectors, and fuses using motor stall current and battery voltage. PCB current capacity is unverified.
- Use charging, storage, and physical protection appropriate to the battery pack.
