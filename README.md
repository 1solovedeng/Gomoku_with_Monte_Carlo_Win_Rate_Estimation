# Gomoku with Monte Carlo Win Rate Estimation

源项目：[https://github.com/lxie27/Gomoku](https://github.com/lxie27/Gomoku)

本项目在原始 SDLProject 的基础上，添加了“蒙特卡洛随机模拟”胜率估算功能。每次玩家落子后，程序随机模拟 500 局并统计黑白双方胜率，在终端打印输出结果。

---

## 功能

- **下五子棋**：在 15×15 棋盘上两个玩家轮流点击鼠标左键落子。  
- **蒙特卡洛胜率估算**：每次落子后立即在控制台输出：
  ```
  这一步后的蒙特卡洛模拟胜率: 黑: xx%, 白: yy%
  ```
- **界面渲染**：基于 SDL2 绘制棋盘与棋子，不影响图形界面体验。

---

## 我的改动

- 在 `Board::attemptAdd` 中调用 `simulateGames(500)`，模拟 500 局随机对弈。  
- 新增 `Board::simulateGames` 函数，实现纯随机模拟、胜率统计。  
- 在 `Game::init` 中初始化随机种子 `srand(time(NULL))`。  
- 终端输出黑白胜率，不影响 SDL 窗口显示。

---

## 环境配置指南 🛠️

### 🔵 Linux（Ubuntu）
```bash
sudo apt update
sudo apt install cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

### 🍎 macOS（基于 Homebrew）
```bash
brew update
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

### 🪟 Windows（推荐使用 Visual Studio / VSCode）
1. 安装 Visual Studio 生成工具（或 VS2022）。  
2. 下载并解压 SDL2、SDL2_image、SDL2_mixer、SDL2_ttf 的 MSVC 版本。  
3. 将 SDL 主文件夹的 `include` 与 `lib\x64` 路径添加到系统环境变量 `PATH`。  

---

欢迎 Fork 与 Star！
