# Hotdogs

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Vulkan API](https://img.shields.io/badge/Vulkan-AC162C.svg?style=for-the-badge&logo=vulkan&logoColor=white&logoSize=auto)

---

## Introduction

This project is our solution to the [Wyścigi](https://science-cup.pl/wp-content/uploads/2025/01/MSC5_2025_Wyscigi.pdf) task of Motorola Science Cup 2025 contest.

The task required making a 2D racing game from scratch, but we took it a step further by developing a full-fledged 3D game using our custom game engine and physics system.

### Team members:

- [Krzysztof M.](https://github.com/kminek2)
- [Kamil P.](https://github.com/kamix-08)
- [Paweł B.](https://github.com/pawello09)
- [Stanisław F.](https://github.com/fistaszek2009)
- [Filip G.](https://github.com/ffenix7)

## Features:

- [x] realistic physics
- [x] procedural maps
- [x] challenging opponents
- [x] interactive powerups and obstacles
- [x] multiple camera views (1st person, top-down, 3rd person)
- [x] 3D world with cities, trees, animals and more
- [x] engaging music and soundtracks

### Gameplay:

You can use the following keybinds to steer your car:

KEY | ACTION
---|---
W | move forward
S | move backward
A | steer left
D | steer right
E | use bombs
LCTRL | use nitro
SPACE | handbrake

You can navigate the menu by using:

KEY | ACTION
---|---
W | move up
S | move down
A | move left
D | move right
ENTER | select
ESC | go back

#### Debug mode:

You can enable _debug mode_ in the settings. 

It adds the following keybinds:

KEY | ACTION
---|---
R | end the race
N | add nitro
B | add bomb

## Installation

Clone the project by running:

```bash
git clone git@github.com:kminek2/hotdogsrun.git
cd hotdogsrun
```

Then build the project by running:

```bash
cmake --preset vcpkg
cmake --build build
./build/Hotdogs
```
