# 🐔 Chicken Invaders - 2D Arcade Shooter Game

## 🎮 Overview

This project is a 2D arcade-style shooter game inspired by the classic **Chicken Invaders** game. The game implements fundamental computer graphics concepts such as real-time rendering, object transformations, collision detection, and interactive gameplay mechanics. The player controls a spaceship that is dynamically built from multiple components and must survive enemy waves while managing construction costs and achieving the highest possible score.

---

## 🚀 Features

### Spaceship Construction

The player's spaceship is created by combining multiple components rather than using a single predefined object.

Implemented components include:

- Main ship body
- Wings
- Engines
- Weapons
- Additional elements

Each component contributes to the final spaceship structure and has an associated cost that is considered during gameplay.

---

### 🐔 Enemy Waves System

The game includes a dynamic enemy wave generation system:

- Multiple waves of enemies
- Increasing difficulty over time
- Dynamic enemy positioning and movement
- Continuous gameplay progression

The player must survive each wave and efficiently use available resources.

---

### 💥 Combat System

Implemented gameplay mechanics:

- Player movement and input handling
- Projectile shooting system
- Collision detection between projectiles and enemies
- Enemy destruction
- Player state management

The game loop ensures smooth frame updates and responsive controls.

---

## 🖥️ Computer Graphics Concepts

The project demonstrates the following concepts:

- Real-time rendering
- 2D transformations:
  - Translation
  - Rotation
  - Scaling
- Object composition and hierarchy
- Collision detection
- Dynamic object generation
- Frame-based animation

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| A / Left Arrow | Move left |
| D / Right Arrow | Move right |
| W / Up Arrow | Move up |
| S / Down Arrow | Move down |
| Space | Shoot |
| P | Pause |

---

## 🛠️ Technologies Used

- C++
- OpenGL
- GLSL Shaders
- CMake
- Computer Graphics techniques
