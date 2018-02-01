# the-seer

C++ and SDL1.2 isometric adventure and platforming game, almost complete but abandoned. The funniest part is that the game was written one-handed as a consequence of a nasty accident.

I really spent a lot with this one and I'd really love to come back to it and finish the story.

This repository includes the code, data and even planning files, but does not include lots and lots of pictures depicting each different room. I could add those some other time.

## Controls

- Arrow keys to move.
- Space to jump.
- Left shift to run.
- Tab to status/map.
- Left control to use/open.
- Left alt to use secret powers once you get them.
- There are a few more "devel mode" controls there, please, ignore them and don't ruin the fun.

## Building.

This depends on a libdanSDL library related to SDL1.2. , whose implementation files are lost to time. It has been patched with the library present in pigs-n-cows, but things may fail.

- 1 Build the library in motor_sdl.
- 2 Copy the resulting libdansdl.a file to the project root.
- 3 make the project.
- 4 play.
