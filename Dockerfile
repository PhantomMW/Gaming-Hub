FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . /workspace

CMD ["bash"]