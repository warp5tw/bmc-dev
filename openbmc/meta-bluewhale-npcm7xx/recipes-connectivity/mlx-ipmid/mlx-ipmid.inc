DESCRIPTION = "This file specifies configuration for Mellanox IPMI daemon."
SECTION = "console/network"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"
DEPENDS = " \
    openssl \
    popt \
    zlib \
    readline \
    ncurses \
    glib-2.0 \
    gdbm \
    "

INC_PR = "r2"
inherit autotools gettext update-alternatives systemd pkgconfig

PACKAGECONFIG ??= "net-snmp"

PACKAGECONFIG[net-snmp] = ",--with-ucdsnmp=no,net-snmp"
PACKAGECONFIG[python] = "--with-python,--without-python,python,"
PACKAGECONFIG[perl] = "--with-perl=${STAGING_BINDIR_NATIVE},--with-perl=no,perl,"
PACKAGECONFIG[tcl] = "--with-tcl,--without-tcl,tcl,"

EXTRA_OECONF = "\
    --prefix=${prefix} \
    --with-mlxbw \
    "

RDEPENDS_${PN} = "bash"

S = "${WORKDIR}/git"

PARALLEL_MAKEINST = ""

SYSTEMD_PACKAGES = "${PN}"

SYSTEMD_SERVICE_${PN} = " mlx_ipmid.service"

do_install_append() {
    install -d ${D}/etc/init.d
    install -d ${D}/${systemd_unitdir}/system
    install -m 644 ${WORKDIR}/mlx_ipmid.service \
            ${D}/${systemd_unitdir}/system/mlx_ipmid.service
    install -d ${D}${libdir}/mlx_ipmid
    install -m 0644 ${B}/lanserv/mlxbw/.libs/mlxbw.so ${D}${libdir}/mlx_ipmid/
    install -Dpm 644 ${D}/var/mlx_ipmid/mlxbw/71-mlxbw-events.rules \
            ${D}/lib/udev/rules.d/71-mlxbw-events.rules
    install -Dpm 644 ${D}/var/mlx_ipmid/mlxbw/71-mlxbw-hwmon-events.rules \
            ${D}/lib/udev/rules.d/71-mlxbw-hwmon-events.rules
}

FILES_${PN} += "/usr/lib/perl5 \
		/usr/lib/mlx_ipmid \
                ${systemd_unitdir} \
                "

FILES_${PN}-dev += "/usr/lib/OpenIPMI"

FILES_${PN}-dbg += "/usr/lib/perl5/auto/OpenIPMI/.debug"
