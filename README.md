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

## ⚖️ License
This project is developed as part of the 42 School curriculum.
