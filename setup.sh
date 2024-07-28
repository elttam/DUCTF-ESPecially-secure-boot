#!/usr/bin/env bash

# gather challenge files
git clone https://github.com/DownUnderCTF/Challenges_2024_Public.git
mkdir -p challenge
tar --strip-components=1 -zxvf Challenges_2024_Public/pwn/especially-secure-boot/publish/especially-secure-boot.tar.gz -C challenge

# used for persisting ghidra repo etc.
mkdir -p shared
cp challenge/flash-base.bin shared/

# build images
docker compose build
