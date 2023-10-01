## tldr
there are windows and linux binaries in `releases` section
## supported operating systems
windows (x86) and linux (x86_64) (see below)
## build requirements
build is possible under any popular linux distro or `WSL` with following packages

fedora:
```
sudo dnf install libglvnd-devel glew-devel glfw-devel glm-devel
```
ubuntu (i don't use ubuntu so this list may be incorrect):
```
sudo apt install libgl-dev libglew-dev libglfw3-dev libglm-dev
```
## build
simply use command
```
make
```
this will generate `physicstest` in bin/lin64 and `physicstest.exe` in bin/win32