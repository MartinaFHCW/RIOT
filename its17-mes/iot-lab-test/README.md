# MES IoT-Lab Uebung by Fabian Würfl

## Screenshots
* Screenshots can be found in ./screenshots
* They are named mes-ue01-X.png, where X is the number of the screenshot.
* I'll use these numbers for referencing

## What did I do?
* compiled the application via
```
$ cd RIOT/examples/gnrc_networking
$ make BOARD=samr21-xpro
```
* Created a new experiment:
    * name `mesUe02`
    * duration `10 minutes`
    * nodes: `grenoble 368 and 369`
    * see screenshot 1
    * uploaded the gnrc_networking.elf firmware (screenshot 2)
    * associated both nodes with the firmware (screenshot 3)
    * submitted (screenshot 4) -> id 52709
* experiment is listed in my dashboard and started to run instantly because
i have chosen free nodes (screenshot 5)
* experiment failed (see screenshot 6)
    * apparently deployment failed but no error message
    * log file is not very helpful either
