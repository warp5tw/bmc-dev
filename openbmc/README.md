# NPCM750 RunBMC Olympus
This is the Nuvoton RunBMC Olympus System layer. The NPCM750 is an ARM based SoC with external DDR RAM and
supports a large set of peripherals made by Nuvoton.
More information about the NPCM7XX can be found
[here](http://www.nuvoton.com/hq/products/cloud-computing/ibmc/?__locale=en).

- Working with [openbmc master branch](https://github.com/openbmc/openbmc/tree/master "openbmc master branch")

# Dependencies
![](https://cdn.rawgit.com/maxdog988/icons/61485d57/label_openbmc_ver_master.svg)

This layer depends on:

```
  URI: git@github.com:Nuvoton-Israel/openbmc
  branch: runbmc
```

# Contacts for Patches

Please submit any patches against the meta-runbmc-nuvoton layer to the maintainer of nuvoton:

* Joseph Liu, <KWLIU@nuvoton.com>
* Medad CChien, <CTCCHIEN@nuvoton.com>
* Tyrone Ting, <KFTING@nuvoton.com>
* Stanley Chu, <YSCHU@nuvoton.com>
* Tim Lee, <CHLI30@nuvoton.com>
* Brian Ma, <CHMA0@nuvoton.com>
* Jim Liu, <JJLIU0@nuvoton.com>

# Table of Contents

- [Dependencies](#dependencies)
- [Contacts for Patches](#contacts-for-patches)
- [Features](#features)
  * [PSU Manager](#psu-manager)
- [Modifications](#modifications)

# Features

### PSU Manager

Cold redundancy reduces system idle input power by putting these redundant supplies into an almost off (standby) condition or “cold redundancy” mode, as we call it here at Intel®. Cold redundancy has the ability to put the redundant supplies into a standby state to save energy at system idle while still being able to turn them back on fast enough in case of a failure to keep the system operating normally.

When the power subsystem is in Cold Redundant mode; only the power supplies needed to support the best power
delivery efficiency are ON. Any additional power supplies, including the
redundant power supply, are in Cold Standby state.
The Power supplies which support Cold Redundancy will have a register to
maintain rotation ranking order, the PSU with first ranking order will enter
standby mode first.

To support the Power Supply Cold Redundancy feature, BMC needs to check and
rotate ranking orders, the BMC also needs to create some interface for IPMI and
Redfish commands to set or get configurations of Cold Redundancy.

> _For more information, please refer to [@ IDF 2009: Cold Redundancy – A New Power Supply Technology for Reducing System Energy Usage](https://itpeernetwork.intel.com/idf-2009-cold-redundancy-a-new-power-supply-technology-for-reducing-system-energy-usage/) and [psu-cold-redundancy.md](https://gerrit.openbmc-project.xyz/c/openbmc/docs/+/27637/10/designs/psu-cold-redundancy.md)_.

**Source URL**

* [https://github.com/Intel-BMC/provingground/tree/master/psu-manager](https://github.com/Intel-BMC/provingground/tree/master/psu-manager)
* [https://github.com/Intel-BMC/openbmc/blob/intel/meta-intel/meta-common/recipes-intel/ipmi/intel-ipmi-oem_git.bb](https://github.com/Intel-BMC/openbmc/blob/intel/meta-intel/meta-common/recipes-intel/ipmi/intel-ipmi-oem_git.bb)
* [https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/psu-manager/0001-psu-manager-bringup-and-port-intel-ipmi-oem.patch](https://github.com/NTC-CCBG/bmc-dev/tree/master/openbmc/psu-manager/0001-psu-manager-bringup-and-port-intel-ipmi-oem.patch)

**How to use**

1. Add the layer meta-intel into the bblayer.conf in the openbmc local build/conf folder.
2. Download and apply the 0001-psu-manager-bringup-and-port-intel-ipmi-oem.patch in the openbmc root folder. For example:
    ```
    git am 0001-psu-manager-bringup-and-port-intel-ipmi-oem.patch
    ``` 
3. Do not build rsyslog by renaming meta-intel/meta-common/recipes-extended/rsyslog/rsyslog_%.bbappend.
4. Do not build phosphor-ipmi-host by renaming meta-intel/meta-common/recipes-phosphor/ipmi/phosphor-ipmi-host_%.bbappend.

**Known issue**

1. Intel PSU manager is not compatible with the Olympus PSU.
    > _Intel PSU manager uses the command **0xd0** to communicate with the PSU which supports Intel cold redundancy feature to set the cold redundancy order in the PSU._

    > _However, **0xd0** is also a custom command required by Olympus PSU software interface specification to enable/dsiable PSU to enter battery backup mode when AC is loss._

**IPMI command**

1. Get Cold Redundancy - CR feature
    ```
    ipmitool -I lanplus -H xx.xx.xx.xx -U root -P 0penBmc raw 0x30 0x2e 0x01
    ```
2. Set Cold Redundancy - CR feature disable
    ```
    ipmitool -I lanplus -H xx.xx.xx.xx -U root -P 0penBmc raw 0x30 0x2d 0x01 0x00
    ```
3. Set Cold Redundancy - CR feature (re-)enable
    ```
    ipmitool -I lanplus -H xx.xx.xx.xx -U root -P 0penBmc raw 0x30 0x2d 0x01 0x01
    ```

    > _For more information, please refer to [How to disable the cold redundancy](https://www.intel.com/content/www/us/en/support/articles/000025905/server-products/server-boards.html)_


**Maintainer**

* Tyrone Ting


# Modifications

* 2020.03.25 First release ReadME.md
