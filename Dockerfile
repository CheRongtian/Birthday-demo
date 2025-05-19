FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# 安装编译器和 SFML 开发包
RUN apt update && apt install -y \
    g++ libsfml-dev \
    && apt clean

# 设置工作目录
WORKDIR /app

# 拷贝项目文件
COPY . .

# 默认运行：编译 text.cpp 到当前目录
ENTRYPOINT ["g++", "text.cpp", "-o", "birthday", "-lsfml-graphics", "-lsfml-window", "-lsfml-system", "-lsfml-audio"]

