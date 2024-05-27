#! /usr/bin/env nix-shell
#! nix-shell -i bash -p bash gnused fd curl coreutils unzip
# shellcheck shell=bash
set -e

COMMIT=552ad5a446a6b22c21e8c7856cfe967e10d5b364

fail()
{
    echo "$1" >&2
    exit 1
}

make_patch()
{
    temp_dir="$(mktemp -d)"
    
    pushd "$temp_dir"
    curl -sL "https://github.com/CommanderBubble/MD5/archive/$COMMIT.zip" -o "MD5-$COMMIT.zip"
    unzip "MD5-$COMMIT.zip"
    pushd "MD5-$COMMIT"

    git init
    git add .

    sed -i'' -e '/const unsigned int UINT32_MAX/d' src/md5_loc.h

    git diff > "$WORKSPACE_ROOT/third_party/md5/md5.patch"

    popd # src dir
    popd # temp dir

    rm -rf "$temp_dir"
}

[[ -f WORKSPACE ]] || fail "Must be executed from the workspace root"

WORKSPACE_ROOT="$PWD"

echo "Patching..."

make_patch 2>/dev/null

ls third_party/md5/md5.patch