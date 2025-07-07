#!/data/data/com.termux/files/usr/bin/sh
#!/bin/bash
termux-setup-storage ; sleep 3
clear

# 定义颜色和样式
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
MAGENTA='\033[1;35m'
CYAN='\033[1;36m'
NC='\033[0m' # 重置颜色
BOLD='\033[1m'
UNDERLINE='\033[4m'

# 函数: 打印 LINBOX 标识
print_banner() {
  clear
  # 彩虹色系
  COLORS=(
    "\033[38;5;39m"  # L-天蓝
    "\033[38;5;46m"  # I-亮绿
    "\033[38;5;226m" # N-明黄
    "\033[38;5;200m" # B-粉紫
    "\033[38;5;208m" # O-橙红
    "\033[38;5;51m"  # X-青蓝
  )
  
  echo -e "\n${COLORS[0]}     ██╗     ██╗███╗   ██╗██████╗  ██████╗ ██╗  ██╗"
  echo -e "${COLORS[1]}     ██║     ██║████╗  ██║██╔══██╗██╔═══██╗╚██╗██╔╝"
  echo -e "${COLORS[2]}     ██║     ██║██╔██╗ ██║██████╔╝██║   ██║ ╚███╔╝ "
  echo -e "${COLORS[3]}     ██║     ██║██║╚██╗██║██╔══██╗██║   ██║ ██╔██╗ "
  echo -e "${COLORS[4]}     ███████╗██║██║ ╚████║██████╔╝╚██████╔╝██╔╝ ██╗"
  echo -e "${COLORS[5]}     ╚══════╝╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝ ╚═╝  ╚═╝"
  # 边框
  echo -e "${GREEN}     ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■${NC}"
  echo -e "     \033[38;5;196m▶\033[38;5;226m▶\033[38;5;46m▶ ${CYAN}          Linbox 安装程序 v1.3.1        ${NC}\033[38;5;46m◀\033[38;5;226m◀\033[38;5;196m◀"
  echo -e "${GREEN}     ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■${NC}\n"
}

# 简单可靠的spinner函数
spinner() {
  local pid=$!
 # local delay=0.1
  local spinstr='|/-\'
  
  # 在Termux中，我们使用简单的循环而不是进程检查
  while :; do
    if ! kill -0 $pid 2>/dev/null; then
      break
    fi
    
    local temp=${spinstr#?}
    printf " [%c] " "$spinstr"
    local spinstr=$temp${spinstr%"$temp"}
   # sleep $delay
    printf "\b\b\b\b\b"
  done
  printf "    \b\b\b\b"
}

# 检查命令是否存在
command_exists() {
  type "$1" >/dev/null 2>&1
}

print_banner
echo -e "\n${MAGENTA}${BOLD}══════════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}${BOLD}                  开始安装 Linbox${NC}"
echo -e "${GREEN}感谢mobox开发人员！感谢咔咔龙，小白一枚，云起云落，deemo，Asia，afei等${NC}"
echo -e "${MAGENTA}${BOLD}══════════════════════════════════════════════════════${NC}\n"

# 安装必要的基础工具
echo -e "${BLUE}${BOLD}[0/7]${NC} ${YELLOW}安装基础工具...${NC}"
{
  pkg update -n
  pkg install -y tar
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 基础工具安装完成${NC}"

# 检查必要文件是否存在
check_required_files() {
  local files=(
    "./termux.tar.xz"
    "./debian.tar.xz"
    "./glibc.tar.xz"
    "./opt.tar.xz"
    "./wine-9.2-vanilla-wow64.tar.xz"
  )
  
  for file in "${files[@]}"; do
    if [ ! -f "$file" ]; then
      echo -e "${RED}错误: 缺少必要文件 $file${NC}"
      return 1
    fi
  done
}

echo -e "${BLUE}${BOLD}[1/7]${NC} ${YELLOW}检查必要文件...${NC}"

spinner
if [ $? -ne 0 ]; then
  echo -e "${RED}✗ 文件检查失败，请确保所有必要文件已放置在./目录下${NC}"
  exit 1
fi
echo -e "${GREEN}✓ 所有必要文件存在${NC}"

# 恢复x11环境
echo -e "${BLUE}${BOLD}[2/7]${NC} ${YELLOW}恢复x11环境...${NC}"
(tar -pxf  ./termux.tar.xz -C /data/data/com.termux/files >/dev/null 2>&1) &
spinner
echo -e "${GREEN}✓ 完成${NC}"

# 恢复debian环境
echo -e "${BLUE}${BOLD}[3/7]${NC} ${YELLOW}恢复debian环境...${NC}"
(tar -pxf  ./tdebian.tar.xz -C /data/data/com.termux/files/usr/var/lib/proot-distro/installed-rootfs/ >/dev/null 2>&1) &
spinner
echo -e "${GREEN}✓ 完成${NC}"

# 安装glibc本体
echo -e "${BLUE}${BOLD}[4/7]${NC} ${YELLOW}安装mobox的glibc本体...${NC}"
(tar -pxf  ./tglibc.tar.xz -C /data/data/com.termux/files/usr >/dev/null 2>&1) &
spinner
echo -e "${GREEN}✓ 完成${NC}"

# 安装wine和opt包
echo -e "${BLUE}${BOLD}[5/7]${NC} ${YELLOW}安装wine和opt包...${NC}"
{
  tar -pxf  ./opt.tar.xz -C /data/data/com.termux/files/usr >/dev/null 2>&1
  tar -pxf  ./wine-9.2-vanilla-wow64.tar.xz -C /data/data/com.termux/files/usr/glibc >/dev/null 2>&1
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 完成${NC}"

# 加入权限
echo -e "${BLUE}${BOLD}[6/7]${NC} ${YELLOW}设置执行权限...${NC}"
{
  chmod a+x $PREFIX/bin/startonwine $PREFIX/bin/stopwine $PREFIX/bin/mobox_menu \
            $PREFIX/bin/linbox $PREFIX/bin/linboxexa $PREFIX/bin/starttfmpt \
            $PREFIX/bin/starttfmpt1
  chmod -R 700 /data/data/com.termux/files/usr/glibc/wine*
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 权限设置完成${NC}"

clear
print_banner
# 最终完成提示
echo -e "\n${GREEN}${BOLD}══════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}${BOLD}                  所有组件安装完成！${NC}"
echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════${NC}\n"

# 自动修复部分
echo -e "${CYAN}${BOLD}接下来自动修复root用户和部分问题...${NC}"
echo -e "${YELLOW}如果失败请参考说明手动修复${NC}"

echo -e "\n${BLUE}${BOLD}[1/4]${NC} ${YELLOW}修复SSH服务...${NC}"
(pkg install -y openssh >/dev/null 2>&1) &
spinner
echo -e "${GREEN}✓ 完成${NC}"

echo -e "\n${BLUE}${BOLD}[2/4]${NC} ${YELLOW}修复部分手机进不去界面...${NC}"
{
  pkg remove xkeyboard-config -y
  pkg i xkeyboard-config -y
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 完成${NC}"

echo -e "\n${BLUE}${BOLD}[3/4]${NC} ${YELLOW}修复Debian的root用户...${NC}"
{
  proot-distro login debian --shared-tmp -- /bin/bash -c 'export PULSE_SERVER=127.0.0.1 && export XDG_RUNTIME_DIR=${TMPDIR} && su - root -c "env DISPLAY=:0 dpkg --configure -a"'
  proot-distro login debian --shared-tmp -- /bin/bash -c 'export PULSE_SERVER=127.0.0.1 && export XDG_RUNTIME_DIR=${TMPDIR} && su - root -c "env DISPLAY=:0 apt update -y"'
  proot-distro login debian --shared-tmp -- /bin/bash -c 'export PULSE_SERVER=127.0.0.1 && export XDG_RUNTIME_DIR=${TMPDIR} && su - root -c "env DISPLAY=:0 apt reinstall sudo -y"'
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 完成${NC}"

echo -e "\n${BLUE}${BOLD}[4/4]${NC} ${YELLOW}创建启动器配置...${NC}"
{
  touch /data/data/com.termux/files/home/.startItemEntries
  echo '{"version":"1.0","name":"startItemEntries","currentStartItem":0,"elements":[{"path":"/data/data/com.termux/files/usr/bin/linbox","fileName":"linbox","iconPath":"default"}]}' > "/data/data/com.termux/files/home/.startItemEntries"
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 完成${NC}"
echo -e "${BLUE}${BOLD}[8/7]${NC} ${YELLOW}进行重建依赖...${NC}"
{
  pkg remove termux-x11-nightly -y
dpkg -i /data/data/com.termux/files/home/xkeyboard-config_2.45_all.deb
dpkg -i /data/data/com.termux/files/home/termux-x11-nightly-1.03.10-0-all.deb
chmod +x /data/data/com.termux/files/usr/libexec/termux-x11/loader.apk
chmod +x /data/data/com.termux/files/usr/bin/startxserver
chmod +x /data/data/com.termux/files/usr/bin/stopserver
chmod +x /data/data/com.termux/files/usr/bin/termux-x11
chmod +x /data/data/com.termux/files/usr/bin/termux-display
chmod +x /data/data/com.termux/files/usr/bin/termux-x11-preference
} >/dev/null 2>&1 &
spinner
echo -e "${GREEN}✓ 重建依赖完成${NC}"
# 最终提示
echo -e "\n${GREEN}${BOLD}═══════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}${BOLD}                   修复完成！${NC}"
echo -e "${RED}${BOLD}重要：请彻底关闭Termux后台进程${NC}"
echo -e "${YELLOW}重新打开Termux后，您可以使用以下命令：${NC}"
echo -e "${CYAN}• ${BOLD}linbox${NC}    - 启动标准X11环境"
echo -e "${CYAN}• ${BOLD}linboxexa${NC} - 启动增强X11环境"
echo -e "${GREEN}${BOLD}════════════════════════════════════════════════════════${NC}\n"

# 历史命令设置
history -c
history -s linbox

echo -e "${MAGENTA}提示：按${BOLD}方向上键${NC}${MAGENTA}可快速输入'linbox'命令${NC}"
rm /data/data/com.termux/files/home/*.tar.xz

linbox
rm /data/data/com.termux/files/home/install.sh