#!/usr/bin/env bash

version=$1
target=cedit++-${version}.msi
cedit_dir=".."
source_dir="installer/SourceDir"
wix_dir="/c/Program Files (x86)/WiX Toolset v3.11"

echo "preparing source directory"
rm -rf ${source_dir}
mkdir ${source_dir} ${source_dir}/bin ${source_dir}/lib ${source_dir}/share ${source_dir}/src

echo "copying necessary files"
cp ${cedit_dir}/bin/cedit.exe ${source_dir}/bin
cp -r ${cedit_dir}/share/locale ${source_dir}/share
cp -r ${cedit_dir}/src/resource ${source_dir}/src
mv ${source_dir}/src/resource/setting/setting installer
cp /mingw64/bin/libatk-1.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libatkmm-1.6-1.dll ${source_dir}/bin
cp /mingw64/bin/libbz2-1.dll ${source_dir}/bin
cp /mingw64/bin/libcairo-2.dll ${source_dir}/bin
cp /mingw64/bin/libcairo-gobject-2.dll ${source_dir}/bin
cp /mingw64/bin/libcairomm-1.0-1.dll ${source_dir}/bin
cp /mingw64/bin/libcroco-0.6-3.dll ${source_dir}/bin
cp /mingw64/bin/libdatrie-1.dll ${source_dir}/bin
cp /mingw64/bin/libepoxy-0.dll ${source_dir}/bin
cp /mingw64/bin/libexpat-1.dll ${source_dir}/bin
cp /mingw64/bin/libffi-6.dll ${source_dir}/bin
cp /mingw64/bin/libfontconfig-1.dll ${source_dir}/bin
cp /mingw64/bin/libfreetype-6.dll ${source_dir}/bin
cp /mingw64/bin/libfribidi-0.dll ${source_dir}/bin
cp /mingw64/bin/libgcc_s_seh-1.dll ${source_dir}/bin
cp /mingw64/bin/libgdk_pixbuf-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libgdk-3-0.dll ${source_dir}/bin
cp /mingw64/bin/libgdkmm-3.0-1.dll ${source_dir}/bin
cp /mingw64/bin/libgio-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libgiomm-2.4-1.dll ${source_dir}/bin
cp /mingw64/bin/libglib-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libglibmm-2.4-1.dll ${source_dir}/bin
cp /mingw64/bin/libgmodule-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libgobject-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libgraphite2.dll ${source_dir}/bin
cp /mingw64/bin/libgtk-3-0.dll ${source_dir}/bin
cp /mingw64/bin/libgtkmm-3.0-1.dll ${source_dir}/bin
cp /mingw64/bin/libgtksourceview-3.0-1.dll ${source_dir}/bin
cp /mingw64/bin/libgtksourceviewmm-3.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libharfbuzz-0.dll ${source_dir}/bin
cp /mingw64/bin/libiconv-2.dll ${source_dir}/bin
cp /mingw64/bin/libintl-8.dll ${source_dir}/bin
cp /mingw64/bin/liblzma-5.dll ${source_dir}/bin
cp /mingw64/bin/libpango-1.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libpangocairo-1.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libpangoft2-1.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libpangomm-1.4-1.dll ${source_dir}/bin
cp /mingw64/bin/libpangowin32-1.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libpcre-1.dll ${source_dir}/bin
cp /mingw64/bin/libpixman-1-0.dll ${source_dir}/bin
cp /mingw64/bin/libpng16-16.dll ${source_dir}/bin
cp /mingw64/bin/librsvg-2-2.dll ${source_dir}/bin
cp /mingw64/bin/libsigc-2.0-0.dll ${source_dir}/bin
cp /mingw64/bin/libstdc++-6.dll ${source_dir}/bin
cp /mingw64/bin/libthai-0.dll ${source_dir}/bin
cp /mingw64/bin/libwinpthread-1.dll ${source_dir}/bin
cp /mingw64/bin/libxml2-2.dll ${source_dir}/bin
cp /mingw64/bin/zlib1.dll ${source_dir}/bin
cp /mingw64/bin/glib-compile-schemas.exe ${source_dir}/bin
cp /mingw64/bin/gtk-update-icon-cache-3.0.exe ${source_dir}/bin
cp /mingw64/bin/gdk-pixbuf-query-loaders.exe ${source_dir}/bin
cp -r /mingw64/lib/gdk-pixbuf-2.0 ${source_dir}/lib
cp -r /mingw64/share/glib-2.0 ${source_dir}/share
cp -r /mingw64/share/icons ${source_dir}/share
cp /mingw64/share/locale/ja/LC_MESSAGES/atk10.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/libpeas.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/gsettings-desktop-schemas.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/json-glib-1.0.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/glib20.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/gdk-pixbuf.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/gtk30.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/gtk30-properties.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES
cp /mingw64/share/locale/ja/LC_MESSAGES/gtksourceview-3.0.mo ${source_dir}/share/locale/ja_JP/LC_MESSAGES

echo "preprocessing"
sed -i "s|VERSION|${version}|g" installer/cedit++.wxs

echo "creating installer"
pushd "installer" > /dev/null
"${wix_dir}/bin/heat.exe" dir SourceDir -gg -dr INSTALLDIR -cg binaries -sfrag -sreg -srd -suid -template fragment -out binaries.wxs
"${wix_dir}/bin/candle.exe" -ext WixUtilExtension cedit++.wxs binaries.wxs
"${wix_dir}/bin/light.exe" -ext WixUtilExtension -ext WixUIExtension -cultures:ja-jp cedit++.wixobj binaries.wixobj -o ${target}
popd > /dev/null

echo "postprocessing"
sed -i "s|${version}|VERSION|g" installer/cedit++.wxs

echo "cleaning up files"
rm -rf installer/SourceDir installer/setting installer/binaries.wxs installer/*.wixobj installer/*.wixpdb
