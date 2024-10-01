FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    ncurses-dev \
    pkg-config \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /project

WORKDIR /brick
COPY src_front /brick/src
COPY start.sh .

ENV TERM=xterm-256color 
ENV TCOLOR=0
ENV TBRIGHT=0

CMD ["sh", "start.sh"]