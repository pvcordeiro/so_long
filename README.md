# So_Long 🎮

A very simple 2D game crafted with MinilibX graphics library, challenging players to navigate through carefully designed maps while collecting items and reaching the exit.

![Screenshot from 2024-12-19 04-29-24](https://github.com/user-attachments/assets/0450b162-cf88-4929-8265-ddc26b6a4574)

## 🎯 Gameplay Overview
In this engaging adventure, players must:
- 🏃 Control a character through various levels
- 💎 Collect all items before finding the exit
- 🧭 Navigate through obstacles and walls
- 👾 Try to finish without dying

## 🎨 Core Features
- Smooth character movement
- Precise collision detection
- Map validation system
- Collectible tracking
- Win/lose conditions

## 👾 Enemy System
The game features challenging enemies that:
- 🤖 Move in unpredictable patterns
- 💥 Must be avoided by the player
- ⚔️ Give damage on contact
- 🎯 Add strategic depth to gameplay

## 🛠️ Technical Implementation
Built using:
- MinilibX graphics library
- C programming language
- Modular code architecture
- Event handling system

## 🎮 Controls
- `W` - Move up
- `S` - Move down
- `A` - Move left
- `D` - Move right
- `Shift` - Sprint
- `Space` - Attack
- `ESC` - Exit game

## 🗺️ Map Format
Maps use `.ber` extension with these elements:
- `1` - Wall
- `0` - Empty space
- `P` - Player starting position
- `E` - Exit
- `C` - Collectible
- `X` - Enemy

## 📝 Map Requirements
- Must be rectangular
- Surrounded by walls
- Contains exactly one exit
- Contains exactly one starting position
- Contains at least one collectible
- Must have a valid path to exit

## 🕹️ How to Play

Follow these steps to get started with **So_Long**:

### 📥 Clone the Repository
1. Open your terminal.  
2. Clone the repository by running:  

```
   git clone https://github.com/yourusername/so_long.git
```

3. Navigate to the project folder:

```
cd so_long
```


🛠️ Build the Game

1. Compile the game using the provided Makefile:

```
make
```


🎮 Start the Game

1. Run the game with:

```
make run
```

2. Alternatively, you can execute the binary directly:

```
./so_long path/to/map.ber
```

Replace path/to/map.ber with the path to your desired map file.



🚀 Enjoy!

Use the controls listed above to navigate the map, collect items, and reach the exit.

Make sure to follow the map format and requirements for custom maps!


## ⚖️ License
This project is developed as part of the 42 School curriculum.
