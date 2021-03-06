#!/usr/bin/env bash

echo `pwd`

VERSION_PATH=../src/c_lib/common/version.h
if [ ! -e ${VERSION_PATH} ]; then
    echo "version.h not found at: " ${VERSION_PATH}
    exit 1
fi

version=`cat ../src/c_lib/common/version.h | grep GS_VERSION | cut -d " " -f 3`
if [[ "$version" == */* ]]; then
    echo "Invalid version:" $version
    exit 1
fi
if [ -z "$version" ]; then
    echo "Invalid version:" $version
    exit 1
fi

./waf configure --release=production --windows
./waf --nocache

f="gnomescroll_windows_"$version
rm -rf "$f"
mkdir "$f"
cp -d run.exe "$f"/gnomescroll.exe
cp -d -R media "$f"/
mkdir "$f"/settings
cp -d ./settings/production.lua "$f"/settings/settings.lua
cp -d ./settings/lua_library.lua "$f"/settings/lua_library.lua
cp -d ./settings/load_options.lua "$f"/settings/load_options.lua
mkdir "$f"/screenshot

#mkdir -p "$f"/lib/win32/
#cp -R -d ../lib/win32/dll "$f"/lib/win32/
#cp -R -d ../lib/win32/lib "$f"/lib/win32/
#cp -R -d ../lib/win32/awesomium "$f"/lib/win32/
#rm -rf "$f"/lib/win32/awesomium/bin/debug

cp ../lib/win32/dll/* "$f"/
#cp -R ../lib/win32/awesomium/bin/release/* "$f"/

if [ -d "$f".zip ]; then
    rm "$f".zip
fi

zip -r "$f".zip "$f"

# copy the package to the server
ssh root@direct.gnomescroll.com 'mkdir /var/www/gnomescroll_downloads/'${version}'/'
scp "$f".zip root@direct.gnomescroll.com:/var/www/gnomescroll_downloads/"$version"/"$f".zip

# Copy to dropbox so we can get a copy to test in the vm
mv "$f".zip ~/Dropbox/Public

if [ -d "f" ]; then
    rm -rf "$f"
fi
