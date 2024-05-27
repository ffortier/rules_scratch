#! /usr/bin/env nix-shell
#! nix-shell -i bash -p bash gnused fd curl coreutils unzip
# shellcheck shell=bash
set -e

ZIPLIB_COMMIT="176e4b6d51fc"

fail()
{
    echo "$1" >&2
    exit 1
}

make_patch()
{
    temp_dir="$(mktemp -d)"
    
    pushd "$temp_dir"
    curl -sLO "https://bitbucket.org/wbenny/ziplib/get/$ZIPLIB_COMMIT.zip"
    unzip $ZIPLIB_COMMIT.zip
    pushd "wbenny-ziplib-$ZIPLIB_COMMIT"

    git init
    git add .

    fd '\.cpp$' --print0 | xargs -0 sed -i'' -e 's/std::ios::streamoff/std::streamoff/g'

    git diff > "$WORKSPACE_ROOT/third_party/ziplib/ziplib.patch"

    popd # src dir
    popd # temp dir

    rm -rf "$temp_dir"
}

[[ -f WORKSPACE ]] || fail "Must be executed from the workspace root"

WORKSPACE_ROOT="$PWD"

echo "Patching..."

make_patch 2>/dev/null

ls third_party/ziplib/ziplib.patch