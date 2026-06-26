# 💣 Minesweeper

A fully-featured Minesweeper built from scratch — starting as a C++ terminal game, now upgraded to a complete web version with dual themes, sound effects, animations, and a live leaderboard.

🎮 **[Play it live →](https://kamer-stack.github.io/Minesweeper-game-/)**

---

## 🌐 Web Version (New!)

Built with vanilla HTML/CSS/JS — no frameworks, no dependencies. Just open and play.

| Feature | Details |
|---|---|
| 🎨 **Dual Themes** | Switch between **DARK** (cyber/neon) and **DREAM** (glassmorphism/purple) |
| 🔊 **Sound Engine** | Web Audio API — reveal, flag, win, lose, hover sounds |
| ⏱ **Timer** | Live game timer with pause support |
| ⚑ **Flagging** | Right-click or press-and-hold on touch |
| 🏆 **High Scores** | Top 10 per difficulty, sorted by time |
| 📊 **Player Stats** | Wins, losses, games played, average time |
| 💥 **Animations** | Wave flood-fill reveal, explosion effect |
| 📱 **Touch Support** | Fully playable on mobile |
| 🔒 **Safe First Click** | Mines place after your first click — never die immediately |

### Difficulty Levels
| Level | Grid | Mines |
|---|---|---|
| Easy | 6×6 | 5 |
| Medium | 8×8 | 11 |
| Hard | 12×12 | 28 |

---

## 💻 C++ Terminal Version (Original)

Classic Minesweeper built from scratch in C++ with full logic-based gameplay and a clean console interface.

### Features
- Interactive console menu — Play, High Scores, Player Stats, Help, Exit
- Multiple difficulty levels with dynamic board sizes
- Flagging system with limited flags
- Timer and player stats tracking
- High score system saved to file
- Colored console output for numbers, flags, bombs
- Recursive reveal algorithm — auto-expands empty cells
- File handling for persistent scores and stats

### How to Run
```bash
git clone https://github.com/kamer-stack/Minesweeper-game-.git
cd Minesweeper-game-
g++ minesweeper.cpp -o minesweeper
./minesweeper
```

---

## 🛠 Tech Stack

| Version | Tech |
|---|---|
| Web | HTML5, CSS3 (CSS Variables, Glassmorphism, Animations), Vanilla JS, Web Audio API |
| Terminal | C++, File I/O, ANSI Colors, Recursive Algorithms |

---

## 📁 Project Structure
Minesweeper-game-/

├── index.html        ← Web version (play in browser)

├── minesweeper.cpp   ← Original C++ terminal version

└── README.md

---

Made by [kamer-stack](https://github.com/kamer-stack)
