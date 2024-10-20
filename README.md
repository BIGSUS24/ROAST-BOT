# ROAST BOT - Discord Bot

ROAST BOT is a simple Discord bot built in C++ that responds to messages with random insults and roasts. The bot can ping users and is designed to be easy to run and modify.

## Requirements

Before you start, ensure you have the following libraries and tools installed:

1. **C++ Compiler**: A C++ compiler that supports C++11 or later (e.g., g++).
2. **libcurl**: A library for transferring data with URLs. You can download it from [libcurl's official website](https://curl.se/libcurl/).
3. **nlohmann/json**: A JSON library for C++. You can find it on [GitHub](https://github.com/nlohmann/json).

### Installation Steps

1. **Install libcurl**:
   - Download and extract the libcurl library.
   - Follow the installation instructions specific to your platform.

2. **Install nlohmann/json**:
   - Download the single-header file `json.hpp` from the [nlohmann/json repository](https://github.com/nlohmann/json).
   - Place it in a suitable include directory (e.g., `C:/json-develop/json-develop/include/nlohmann`).

3. **Set Up Your Discord Bot**:
   - Create a new bot on the [Discord Developer Portal](https://discord.com/developers/applications).
   - Get your bot token and add the bot to your server with appropriate permissions.

4. **Configure the Code**:
   - Replace the `BOT_TOKEN` and `CHANNEL_ID` in the code with your bot's token and the channel ID where you want the bot to operate.

## Build Instructions

To compile the bot, use the following command in your terminal:

```bash
g++ ROAST_BOT.cpp -o ROAST_BOT -I"C:/curl-8.10.1_6-win64-mingw/curl-8.10.1_6-win64-mingw/include" -L"C:/curl-8.10.1_6-win64-mingw/curl-8-10.1_6-win64-mingw/lib" -lcurl -I"C:/json-develop/json-develop/include" -I"C:/json-develop/json-develop/include/nlohmann"
