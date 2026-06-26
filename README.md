# Minesweeper

[![Play Live](https://img.shields.io/badge/Play%20Live-Enjoy-brightgreen)](https://kamer-stack.github.io/Minesweeper-game-/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B%20%7C%20JavaScript-blue)]()
[![License](https://img.shields.io/badge/License-MIT-pink)]()

A classic Minesweeper game built from scratch — started as a C++ terminal application, now a fully-featured browser game with dual themes, sound effects, animations, and a live leaderboard.

**[Play it live →](https://kamer-stack.github.io/Minesweeper-game-/)**

---

## Web Version

Built with vanilla HTML, CSS, and JavaScript. No frameworks, no dependencies — just open and play.

| Feature | Details |
|---|---|
| Dual Themes | Switch between DARK (cyber/neon) and DREAM (glassmorphism/purple) at any time |
| Sound Engine | Web Audio API — reveal, flag, win, lose, hover, and click sounds |
| Timer & Pause | Live game timer with full pause and resume support |
| Flagging | Right-click on desktop, press-and-hold on mobile |
| High Scores | Top 10 leaderboard per difficulty, sorted by completion time |
| Player Stats | Wins, losses, games played, and average time per player |
| Animations | Wave flood-fill reveal and mine explosion effects |
| Mobile Support | Fully playable on touch devices |
| Safe First Click | Mines place after your first click — you never lose immediately |

### Difficulty Levels

| Level | Grid | Mines |
|---|---|---|
| Easy | 6×6 | 5 |
| Medium | 8×8 | 11 |
| Hard | 12×12 | 28 |

---

## C++ Terminal Version

The original version — a complete Minesweeper implementation in the console, built entirely in C++ with no external libraries.

### Features

- Interactive console menu: Play, High Scores, Player Stats, Help, Exit
- Three difficulty levels with dynamic board sizes and mine counts
- Flagging system with limited flags per game
- Live timer and per-player stats tracking
- High score system saved to file, top players listed per difficulty
- Colored console output for numbers, flags, and mines
- Recursive reveal algorithm — auto-expands empty regions
- Persistent scores and stats via file I/O

### How to Run

```bash
git clone https://github.com/kamer-stack/Minesweeper-game-.git
cd Minesweeper-game-
g++ minesweeper.cpp -o minesweeper
./minesweeper
```

---

## Tech Stack

| Version | Technologies |
|---|---|
| Web | HTML5, CSS3 (Variables, Glassmorphism, Animations), Vanilla JS, Web Audio API |
| Terminal | C++, ANSI escape codes, File I/O, Recursive algorithms |

## Project Structure

```
Minesweeper-game-/
├── index.html        Web version — open in any browser to play
├── minesweeper.cpp   Original C++ terminal version
└── README.md
```

---

## Contributing

Found a bug or have an idea? Feel free to open an issue or submit a pull request. All contributions are welcome.

1. Fork the repo
2. Create a branch: `git checkout -b feature/your-idea`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push and open a pull request

---

If you like this project, consider giving it a star — it helps more people find it and keeps the motivation going.

[![Star this repo](https://img.shields.io/github/stars/kamer-stack/Minesweeper-game-?style=social)](https://github.com/kamer-stack/Minesweeper-game-)

---

Made by [kamer-stack](https://github.com/kamer-stack)
