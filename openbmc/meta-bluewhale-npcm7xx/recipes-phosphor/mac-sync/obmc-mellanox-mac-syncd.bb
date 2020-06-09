SUMMARY = "OpenBMC Mellanox MAC Sync Daemon"
DESCRIPTION = "OpenBMC Mellanox MAC Sync Daemon implementation."
PR = "r1"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd
inherit autotools

DEPENDS += "systemd"
S = "${WORKDIR}"
SRC_URI += "file://obmc-mellanox-mac-syncd.sh \
           file://obmc-mellanox-mac-syncd.service \
           "

SYSTEMD_SERVICE_${PN} += "obmc-mellanox-mac-syncd.service"

RRECOMMENDS_${PN} += "obmc-targets"

do_compile[noexec] = "1"

do_install() {
        install -d ${D}/${sbindir}
        install -m 755 ${S}/obmc-mellanox-mac-syncd.sh ${D}/${sbindir}/obmc-mellanox-mac-syncd.sh
}
