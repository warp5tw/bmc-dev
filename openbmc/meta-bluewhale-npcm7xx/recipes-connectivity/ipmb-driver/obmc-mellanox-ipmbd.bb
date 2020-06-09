SUMMARY = "OpenBMC Mellanox IPMB driver loader"
DESCRIPTION = "Loads the IPMB driver"
PR = "r1"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd
inherit autotools

DEPENDS += "systemd"
S = "${WORKDIR}"
SRC_URI += "file://obmc-mellanox-ipmbd.sh \
           file://obmc-mellanox-ipmbd.service \
           file://load_bf2bmc_ipmb.sh \
           file://load_bmc2bf_ipmb.sh \
           "

SYSTEMD_SERVICE_${PN} += "obmc-mellanox-ipmbd.service"

RRECOMMENDS_${PN} += "obmc-targets"

do_compile[noexec] = "1"

do_install() {
        install -d ${D}/${sbindir}
        install -m 755 ${S}/obmc-mellanox-ipmbd.sh ${D}/${sbindir}/obmc-mellanox-ipmbd.sh
        install -m 755 ${S}/load_bf2bmc_ipmb.sh ${D}/${sbindir}/load_bf2bmc_ipmb.sh
        install -m 755 ${S}/load_bmc2bf_ipmb.sh ${D}/${sbindir}/load_bmc2bf_ipmb.sh
}
