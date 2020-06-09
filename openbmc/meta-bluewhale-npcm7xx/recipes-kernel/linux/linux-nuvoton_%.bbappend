FILESEXTRAPATHS_prepend := "${THISDIR}/linux-nuvoton:"

SRC_URI_append = " file://bluewhale-npcm7xx.cfg"
SRC_URI_append = " \
  file://0001-dts-nuvoton-add-buv-runbmc-support.patch \
  file://0002-move_emc_message_debug.patch \
  file://0001-add-Mellanox-CPLD-BlueWhale-I2C-driver.patch \
  file://0001-add-mellanox-ipmb-driver.patch \
  file://0001-add-Mellanox-rshim-driver.patch \
  "
