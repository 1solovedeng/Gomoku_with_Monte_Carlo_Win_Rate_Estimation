# 蒙特卡洛算法实现AI五子棋对弈(Gomoku AI with Monte Carlo Simulation)

源项目：[https://github.com/lxie27/Gomoku](https://github.com/lxie27/Gomoku)

本项目在原始 Gomoku-Master 的基础上，添加了“蒙特卡洛随机模拟”胜率估算功能。每次玩家落子后，程序随机模拟 500 局并统计黑白双方胜率，在终端打印输出结果。

---

## 功能

- **下五子棋**：在 15×15 棋盘上玩家点击鼠标左键落子。  
- **蒙特卡洛胜率估算**：每次落子后立即在控制台输出：
  ```
  这一步后的蒙特卡洛模拟胜率: 黑: xx%, 白: yy%
  ```
- **界面渲染**：基于 SDL2 绘制棋盘与棋子，不影响图形界面体验。
![演示图](https://res.cloudinary.com/ddacx9czn/image/upload/v1747838315/2327929c-a670-4707-9e56-53537fe04d5e.png)
---

## 我的改动

- 在 `Board::attemptAdd` 中调用 `simulateGames(500)`，模拟 500 局随机对弈。  
- 新增 `Board::simulateGames` 函数，实现纯随机模拟、胜率统计。  
- 在 `Game::init` 中初始化随机种子 `srand(time(NULL))`。  
- 终端输出黑白胜率，不影响 SDL 窗口显示。
![演示图2](https://res.cloudinary.com/ddacx9czn/image/upload/v1747838388/6b2ac052-cebe-417a-81e1-f5605b3b0f6f.png)
- 添加`CMake`部分，可以在不同操作系统上生成对应文件，方便编译。

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
## 更新时间：2025/5/22
1. 添加 AI：使用蒙特卡洛模拟 200 局，选择最高胜率位置落子。
![与AI对弈演示图](https://res.cloudinary.com/ddacx9czn/image/upload/v1747917253/08771634-f5e2-42f4-9a71-3f9198f9a70d.png)
* 与AI对弈演示图
2. 添加黑棋走子后流程：黑棋(玩家)落子 → 界面刷新 → 控制台打印“白棋 AI 正在思考...” → 延迟后显示白棋 AI 落子位置。
![对应的控制台内容](https://res.cloudinary.com/ddacx9czn/image/upload/v1747917361/3169b4fc-a63d-4dba-9075-2bc384d35672.png)
* 对应的控制台内容图
---

欢迎 Fork 与 Star！
