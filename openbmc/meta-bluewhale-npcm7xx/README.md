# Mellanox BlueField NPCM7XX

An experimental layer runs on Nuvoton RunBMC BUV board

# How to build this layer

```
export TEMPLATECONF=meta-mellanox/meta-mellanox/conf
. openbmc-env
bitbake obmc-phosphor-image
```

# Table of Contents
- [Features of Mellanox BlueField](#features-of-mellanox-bluefield)
  * [Mellanox IPMID](#mellanox-ipmid)
  * [Fan Control](#fan-control)
  * [MAC Address Sync](#mac-address-sync)
  * [Recovery Mode](#recovery-mode)
  * [BMC FW Update](#bmc-fw-update)
  * [Mellanox Si5341/Si52142 programming tool](#mellanox-si5341/si52142-programming-tool)
  * [Mellanox CPLD access](#mellanox-cpld-access)
  * [Mellanox CPLD programming tool](#mellanox-cpld-programming-tool)
  * [Mellanox String](#mellanox-string)
  * [Mellanox NCSI OEM command](#mellanox-ncsi-oem-command)
  * [Mellanox BMC Sanity](#mellanox-bmc-sanity)
  * [Mellanox Power control](#mellanox-power-control)
  * [Mellanox voltage margin](#mellanox-voltage-margin)
  * [IPMB driver](#ipmb-driver)
  * [RSHIM driver](#rshim-driver)
  * [I2C devices list](#i2c-devices-list)

# Features of Mellanox BlueField

## Mellanox IPMID

A IPMI server using OpenIPMI
* [mlx-ipmid](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-connectivity/mlx-ipmid)

**How to use**

```
ipmitool -H 127.0.0.1 -U root -P 0penBmc -I lanplus -C 3 sdr

buv_board        | 32 degrees C      | ok
bmc_card         | 39 degrees C      | ok
fan1_rpm         | 0 RPM             | cr
fan2_rpm         | 0 RPM             | cr
fan3_rpm         | 0 RPM             | cr
fan4_rpm         | 0 RPM             | cr
```

```
ipmitool -H 127.0.0.1 -U root -P 0penBmc -I lanplus -C 3 sel list

   1 |  Pre-Init  |0000000053| Fan #0x03 | Lower Critical going low  | Asserted
   2 |  Pre-Init  |0000000053| Fan #0x04 | Lower Critical going low  | Asserted
   3 |  Pre-Init  |0000000053| Fan #0x05 | Lower Critical going low  | Asserted
   4 |  Pre-Init  |0000000053| Fan #0x06 | Lower Critical going low  | Asserted
```

```
ipmitool -H 127.0.0.1 -U root -P 0penBmc -I lanplus -C 3 fru

FRU Device Description : Builtin FRU Device (ID 0)
 Board Mfg Date        : Wed Jan 23 13:50:00 2019 UTC
 Board Mfg             : Quanta
 Board Product         : F0B-BMC-BMC
 Board Serial          : JU590800012
 Board Part Number     : 3CF0BRB0040
 Board Extra           : Nuvoton NPCM750
 ```
 ```
 ipmitool -H 127.0.0.1 -U root -P 0penBmc -I lanplus -C 3 mc info

Device ID                 : 1
Device Revision           : 1
Firmware Revision         : 2.08
IPMI Version              : 2.0
Manufacturer ID           : 33049
Manufacturer Name         : Mellanox Technologies LTD
Product ID                : 2 (0x0002)
Product Name              : Unknown (0x02)
Device Available          : yes
Provides Device SDRs      : yes
Additional Device Support :
    Sensor Device
    SDR Repository Device
    SEL Device
    FRU Inventory Device
    IPMB Event Receiver
    Chassis Device
Aux Firmware Rev Info     :
    0x00
    0x00
    0x00
    0x00
```
```
ipmitool -H 127.0.0.1 -U root -P 0penBmc -I lanplus -C 3 lan print

Set in Progress         : Set Complete
Auth Type Support       : NONE MD2 MD5 PASSWORD
Auth Type Enable        : Callback :
                        : User     :
                        : Operator :
                        : Admin    :
                        : OEM      :
IP Address Source       : DHCP Address
IP Address              : 192.168.0.12
Subnet Mask             : 255.255.0.0
MAC Address             : 00:00:f7:a0:ff:fd
SNMP Community String   : public
IP Header               : TTL=0x00 Flags=0x00 Precedence=0x00 TOS=0x00
Default Gateway IP      : 192.168.0.254
Default Gateway MAC     : 00:00:00:00:00:00
Backup Gateway IP       : 0.0.0.0
Backup Gateway MAC      : 00:00:00:00:00:00
802.1q VLAN ID          : Disabled
802.1q VLAN Priority    : 0
RMCP+ Cipher Suites     : 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
Cipher Suite Priv Max   : aaaaaaaaaaaaaaa
                        :     X=Cipher Suite Unused
                        :     c=CALLBACK
                        :     u=USER
                        :     o=OPERATOR
                        :     a=ADMIN
                        :     O=OEM
Bad Password Threshold  : Not Available
```

## Fan Control

Monitoring BlueField internal temp(via NIC I2C) and board tempture
* [obmc-mellanox-fand.sh](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/fans)

```
 Fan speed is based on the following board sensor temp ranges:
 0 (error case) : 100% duty cycle (max speed - default setting)
 less than 14999: 25% duty cycle
 15000 to 29999 : 50% duty cycle
 30000 to 44999 : 75% duty cycle
 Above 45000    : 100% duty cycle (max speed)
 Note that temperature readings are interpreted as (value/1000) deg C
 i.e. A temperature reading of 15000 corresponds to 15 deg C
```

## MAC Address Sync

Synchronize MAC address information
* [obmc-mellanox-mac-syncd.sh](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/mac-sync)

```
It only works on booting from primary flash

This script utilizes fw_printenv and fw_setenv to synchronize
MAC address information from primary U-Boot environment to the
backup U-Boot environment.

The MAC address 'eth2addr' for RSHIM is not synced.
```

## Recovery Mode

Dual flash Support

* [init](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/initfs/files/obmc-init.sh)
* [failsafe](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/initfs/files/recovery.sh)
* [recovery](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/initfs/files/failsafe-boot.sh)

```
If BMC booted from backup flash, BMC should enter fail safe mode to recovery.

init:  check boot from which flash

failsafe: fail safe mode (openWRT ??)
    
        setenv bootargs ${bootargs} failsafe=1

recovery: re-burn primary flash with /run/initramfs/recovery /tmp/<bmc-image-file> (It's not recovery from backup flash)

```

## BMC FW Update

Script to burn entire 32MB of BMC SPI Flash

* [update_all](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-phosphor/initfs/files/obmc-update_all.sh)
```
Usage:
    a) Local: /run/initramfs/update_all <bmc-image-file> [-quiet]
    b) Remote: sshpass -p "<root-password>" ssh root@<ip> 
                  '/run/initramfs/update_all <bmc-image-file> [-quiet]'

    Set eth0/eth1 MAC from eeprom if Valid MAC1 env variable does not exist
```

## Mellanox Si5341/Si52142 programming tool

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)
```
Usage: mlnx_si5341prog OPTIONS
   E.g. mlnx_si5341prog --device 9:0x74 -f --setconf config.txt --verify
```

## Mellanox CPLD access

CPLD access tool

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
mlnx_cpldaccess [-mb | -bp] [-r <addr> | -w <addr> <data> | -d]
-mb              : access motherboard cpld
-bp              : access backplane cpld
-r <addr>        : read from <addr>
-w <addr> <data> : write <data> to <addr>
-d               : dump contents of all registers
```

## Mellanox CPLD programming tool

Script to program CPLD firmware

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
SVF location
/usr/share/mellanox/cpld_main_0_13.svf

obmc-update_cpld.sh
Usage:
    a) Local: /run/initramfs/update_cpld <cpld-firmware-file.svf>
    b) Remote: sshpass -p "<root-password>" ssh root@<ip> '/run/initramfs/update_cpld <cpld-firmware-file.svf>'

```

## Mellanox power sequencers programming tool

Script to program the MOBO and BF power sequencers

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
These 2 devices were linked to form a JTAG chain. The TDO of the
MOBO power sequencer becomes the TDI of the BF power sequencer.
TMS and TCK signals are common to both devices in the chain.
So we need only one SVF file to program both as shown below:
 <isp-firmware-file.svf> is a SVF firmware file

```

## Mellanox String

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
mlnx_strings [backup-uboot | backup-linux]

 * For example, "mlnx_strings backup-linux | grep Linux" will display the
 * Linux version string found in the backup Linux image region.

```

## Mellanox NCSI OEM command

send_oem_cmd and recv_oem_rsp 

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
Usage: send_oem_cmd <command> <parameter> [<optional_parameter>]
    Command 		: 0x0 to 0x20
    Parameter		: 0x0 to 0xff
    Optional Parameter	: 0x0 to 0xff

- Open RAW socket   //sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)
- Get the index of the interface //SIOCGIFINDEX
- Get the MAC address of the interface //SIOCGIFHWADDR
- Prepare Packet data 
- Send packet //sendto

```

## Mellanox BMC Sanity
 
A script reports the status of devices connected to the BMC
and checks that those interfaces are operational.

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
mlnx_bmc_sanity.sh
- the BMC I2C devices
- the CPLD switch configurations
- the BMC GPIO pins and state
- the power status
- the temperature status
```

## Mellanox Power control

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
- mlnx_powercycle_bf.sh 
- mlnx_poweron_bf.sh 
- mlnx_poweroff_bf.sh
- mlnx_powerstatus_bf.sh
```


## Mellanox voltage margin

* [mellanox-bmc-tools](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/meta-bluewhale-npcm7xx/recipes-devtools/mellanox-bmc-tools)

```
- mlnx_tps53679_voltage_margin.sh 
- mlnx_tps53915_voltage_margin.sh 
```

## IPMB host driver

* [ipmb host](https://github.com/Mellanox/ipmb-host)
```
1) IPMB requests from the BMC to the BlueField.

   In this mode, the ipmb_dev_int driver needs to be loaded on the
   BlueField.
   The BMC can then send IPMI requests to the BlueField.

2) IPMB requests from the BlueField to the BMC.

   This mode can be enabled via the load_bf2bmc_ipmb.sh script.
   In this mode, the ipmb_host driver needs to be loaded on the
   BlueField. The ipmb_host driver executes a handshake with the
   BMC to be able to load.

At the moment, these 2 modes cannot coexist, so there are 2 scripts
included in this github folder to switch from one mode to another:

1) load_bf2bmc_ipmb.sh - enables sending IPMI requests from the BlueField
   to the BMC

2) load_bmc2bf_ipmb.sh - enables sending IPMI requests from the BMC to the
   BlueField.

```

## RSHIM driver

 virtual console and virtual network interface over rshim

* [rshim driver](https://github.com/Mellanox/rshim)
* [rshim-user-space](https://github.com/Mellanox/rshim-user-space)

## I2C devices list
```
1-0070 -> PCIe Socket switch: Mux which selects PCIe skt0/1
1-0050 -> Riser EEPROM
1-0051 -> Backplane EEPROM
1-0042 -> Backplane CPLD
1-0048 -> Backplane Temp Sense
1-006d -> Zero Delay Buffer - Leave this out for now
1-006b -> Clock Generator - Leave this out for now
1-0071 -> Backplane I2C switch
1-001b -> SSD Management interface
1-0053 -> SSD VPD ROM
3-0058 3-0059 -> AC/DC Power Supply
4-0040 -> CPLD
6-0050 6-0055 -> FRU ROM
7-001f -> DDR 0.9 VRD
7-001b -> BF Serdes 1.2 VRD
7-0040 -> PCIe0 12V monitor
7-0041 -> PCIe0 3.3V monitor
7-0044 -> PCIe1 12V monitor
7-0045 -> PCIe1 3.3V monitor
7-004b 7-004c 7-004d 7-004f -> BT PCIe volt monitor
8-006c -> Temperature sensor
9-0070 9-006b -> clock generator
10-0020 -> MB Power sequencer
10-0021 -> BF Power sequence
11-0058 -> Core and DDR VDD VRD
13-002f -> Fan controller
```