SUMMARY = "Framework for layout and rendering of internationalized text"
DESCRIPTION = "Pango is a library for laying out and rendering of text, \
with an emphasis on internationalization. Pango can be used anywhere \
that text layout is needed, though most of the work on Pango so far has \
been done in the context of the GTK+ widget toolkit. Pango forms the \
core of text and font handling for GTK+-2.x."
HOMEPAGE = "http://www.pango.org/"
BUGTRACKER = "http://bugzilla.gnome.org"
SECTION = "libs"
LICENSE = "LGPL"

SRC_URI = "http://ftp.gnome.org/pub/GNOME/sources/pango/1.28/pango-1.28.4.tar.bz2"

SRC_URI[md5sum] = "3f3989700f04e9117d30544a9078b3a0"
SRC_URI[sha256sum] = "7eb035bcc10dd01569a214d5e2bc3437de95d9ac1cfa9f50035a687c45f05a9f"

X11DEPENDS = "virtual/libx11 libxft"
DEPENDS = "glib-2.0 fontconfig freetype zlib virtual/libiconv gtk-doc-native cairo ${@base_contains('DISTRO_FEATURES', 'x11', '${X11DEPENDS}', '', d)}"

PACKAGES_DYNAMIC = "pango-module-*"

RRECOMMENDS_${PN} = "${@base_contains('DISTRO_FEATURES', 'x11', 'pango-module-basic-x', '', d)} pango-module-basic-fc" 

inherit autotools pkgconfig

EXTRA_AUTORECONF = ""

# seems to go wrong with default cflags
FULL_OPTIMIZATION_arm = "-O2"

EXTRA_OECONF = "--disable-glibtest \
        --enable-explicit-deps=no \
            --disable-debug \
        ${@base_contains('DISTRO_FEATURES', 'x11', '--with-x', '--without-x', d)}"

LEAD_SONAME = "libpango-1.0*"
LIBV = "1.6.0"

postinst_prologue() {
if [ "x$D" != "x" ]; then
  exit 1
fi

}

python populate_packages_prepend () {
    prologue = d.getVar("postinst_prologue", 1)

    modules_root = bb.data.expand('${libdir}/pango/${LIBV}/modules', d)

    do_split_packages(d, modules_root, '^pango-(.*)\.so$', 'pango-module-%s', 'Pango module %s', prologue + 'pango-querymodules > /etc/pango/pango.modules')
}

FILES_${PN} = "/etc ${bindir}/* ${libdir}/libpango*${SOLIBS}"
FILES_${PN}-dbg += "${libdir}/pango/${LIBV}/modules/.debug"
FILES_${PN}-dev += "${libdir}/pango/${LIBV}/modules/*.la"